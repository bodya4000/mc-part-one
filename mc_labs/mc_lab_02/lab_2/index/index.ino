#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

#define SSID "bodyaFrayer"
#define PASSWORD "12345678"
#define BLINK_INTERVAL 500
#define DEBOUNCE_TIME 50
#define DOUBLE_CLICK_TIME 300
#define HOLD_TIME 500

enum class Color { RED, YELLOW, GREEN };
enum class Command { NONE, START, STOP };

typedef struct led_s {
  const uint8_t pin;
  bool state;
  led_s *next;
  led_s *prev;
  Color color;
} led_t;

typedef struct button_s {
  uint8_t pin;
  bool state;
  bool wasPressed;
  uint32_t pressStartTime;
  uint32_t lastClickTime;
  uint32_t lastDebounce;
  uint8_t clickCount;
  bool hardIsHeld;
  bool webIsHeld;
  bool serialIsHeld;
  bool isPressed;
  bool lastState;
} button_t;

led_t redLED = {13, LOW, nullptr, nullptr, Color::RED};
led_t yellowLED = {12, LOW, nullptr, nullptr, Color::YELLOW};
led_t greenLED = {14, LOW, nullptr, nullptr, Color::GREEN};
led_t *currentLED = &redLED;
button_t button = {16, LOW, false, 0, 0, 0, 0, false, false, false, false, HIGH};

uint8_t serialData;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

bool reverseDirection = false;
bool isRunning = false;
uint32_t previousBlinkTime = 0;
Command webCommand = Command::NONE;

// Embedded HTML
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<!DOCTYPE HTML>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="style.css">
</head>
<body>
<h1>ESP Pushbutton Web Server</h1>
<button id="algo1" class="button">
  LED PUSHBUTTON
</button>
<button id="algo2" class="button">
  LED SEND SIGNAL
</button>
<div class="ledsContainer">
  <div class="led" id="red"></div>
  <div class="led" id="yellow"></div>
  <div class="led" id="green"></div>
</div>

<script src="main.js"></script>
</body>
</html>
)rawliteral";

// Embedded CSS
const char style_css[] PROGMEM = R"rawliteral(
body {
  text-align: center;
  margin: 0 auto;
  padding-top: 30px;
  background-color: #c4c4c4;
}
.button {
  padding: 10px 20px;
  font-size: 24px;
  text-align: center;
  outline: none;
  color: #fff;
  background-color: #2f4468;
  border: none;
  border-radius: 5px;
  box-shadow: 0 6px #999;
  cursor: pointer;
  user-select: none;
}
)rawliteral";

// Embedded JavaScript
const char main_js[] PROGMEM = R"rawliteral(
const button1 = document.getElementById("algo1");
const button2 = document.getElementById("algo2");

const holdTime = 500;
let timer1, wasHeldEnough1, timer2, wasHeldEnough2;

button1.addEventListener('mousedown', handleHold);
button1.addEventListener('mouseup', handleRelease);
button1.addEventListener('touchstart', handleHold);
button1.addEventListener('touchend', handleRelease);

button2.addEventListener('mousedown', handleStart);
button2.addEventListener('mouseup', handleStop);
button2.addEventListener('touchstart', handleStart);
button2.addEventListener('touchend', handleStop);

function handleHold() {
  wasHeldEnough1 = false;
  timer1 = setTimeout(() => {
    wasHeldEnough1 = true;
    fetch('/hold').then();
  }, holdTime);
}

function handleRelease() {
  if (wasHeldEnough1) {
    fetch('/release').then();
  }
  clearTimeout(timer1);
  wasHeldEnough1 = false;
}

function handleStart() {
  wasHeldEnough2 = false;
  timer2 = setTimeout(() => {
    wasHeldEnough2 = true;
    fetch('/start').then();
  }, holdTime);
}

function handleStop() {

  if (wasHeldEnough2) {
    fetch('/stop').then();
  }
  clearTimeout(timer2);
  wasHeldEnough2 = false;
}

const socket = new WebSocket("ws://192.168.4.1/ws");

socket.onmessage = (event) => {
  let data = event.data;
  document.querySelectorAll('.led').forEach(led => led.classList.remove('active'));
  if (data === "red") document.getElementById("red").classList.add("active");
  if (data === "yellow") document.getElementById("yellow").classList.add("active");
  if (data === "green") document.getElementById("green").classList.add("active");
}
)rawliteral";

// --- LED logic ---
void setupLEDOrder() {
  redLED.next = &yellowLED; redLED.prev = &greenLED;
  yellowLED.next = &greenLED; yellowLED.prev = &redLED;
  greenLED.next = &redLED; greenLED.prev = &yellowLED;
}

void pinSetup() {
  pinMode(redLED.pin, OUTPUT);
  pinMode(yellowLED.pin, OUTPUT);
  pinMode(greenLED.pin, OUTPUT);
  pinMode(button.pin, INPUT_PULLUP);
}

void lightLEDs() {
  digitalWrite(redLED.pin, redLED.state);
  digitalWrite(yellowLED.pin, yellowLED.state);
  digitalWrite(greenLED.pin, greenLED.state);
}

void checkSerial() {
  if (Serial.available() > 0) {
    serialData = Serial.read();
    switch (serialData) {
      case 'h':
        button.serialIsHeld = true;
        break;
      case 'r':
        button.serialIsHeld = false;
        break;
    }
  }
}


void lightNextLED() {
  uint32_t now = millis();
  if (now - previousBlinkTime >= BLINK_INTERVAL) {
    previousBlinkTime = now;
    currentLED->state = LOW;
    currentLED = reverseDirection ? currentLED->prev : currentLED->next;
    currentLED->state = HIGH;
    lightLEDs();
    switch (currentLED->color) {
      case Color::RED: ws.textAll("red"); break;
      case Color::YELLOW: ws.textAll("yellow"); break;
      case Color::GREEN: ws.textAll("green"); break;
    }
  }
}

void turnOffLEDs() {
  redLED.state = LOW;
  yellowLED.state = LOW;
  greenLED.state = LOW;
  lightLEDs();
}

// --- Button logic ---
// void handleButtonInput(uint32_t now) {
//   bool state = digitalRead(button.pin) == LOW;
//   if (state != button.lastState) {
//     button.lastDebounce = now;
//     button.lastState = state;
//   }

//   if ((now - button.lastDebounce) > DEBOUNCE_TIME) {
//     if (state && !button.isPressed) {
//       button.isPressed = true;
//       if (now - button.lastClickTime < DOUBLE_CLICK_TIME) {
//         button.clickCount++;
//       } else {
//         button.clickCount = 1;
//       }
//       button.lastClickTime = now;
//     } else if (!state && button.isPressed) {
//       button.isPressed = false;
//     }
//   }

//   if (button.clickCount == 2) {
//     isRunning = !isRunning;
//     if (isRunning) {
//       reverseDirection = !reverseDirection;
//       Serial.println("Direction changed (Button)");
//     }
//     button.clickCount = 0;
//   }
// }

void handleButtonInput(uint32_t now) {
  bool state = digitalRead(button.pin) == LOW;

  if (state != button.lastState) {
    button.lastDebounce = now;
    button.lastState = state;
  }

  if ((now - button.lastDebounce) > DEBOUNCE_TIME) {
    if (state && !button.isPressed) {
      button.isPressed = true;
      button.pressStartTime = now;
    } else if (!state && button.isPressed) {
      button.isPressed = false;
      if ((now - button.pressStartTime) >= HOLD_TIME) {
        isRunning = !isRunning;
        Serial.println(isRunning ? "Running started (Hold Button)" : "Running stopped (Hold Button)");
      }
    }
  }

  // ВАЖЛИВО: оновлюй стан утримання кнопки
  button.hardIsHeld = state;
}



// --- Web commands ---
void checkWebCommand() {
  if (webCommand == Command::START) {
    isRunning = true;
    reverseDirection = !reverseDirection;
    Serial.println("Direction changed (Web)");
    webCommand = Command::NONE;
  } else if (webCommand == Command::STOP) {
    isRunning = false;
    webCommand = Command::NONE;
  }
}

// --- Web routes ---
void serverSetup() {
  WiFi.softAP(SSID, PASSWORD);
  // Serial.print("Connected! IP address: ");
  // while (WiFi.status() != WL_CONNECTED) {
  //     delay(500);
  // }
  // Serial.println(WiFi.localIP());
  LittleFS.begin();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/css", style_css);
  });
  server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "application/javascript", main_js);
  });

  server.on("/hold", HTTP_GET, [](AsyncWebServerRequest *request) {
    button.webIsHeld = true;
    request->send(200, "text/plain", "ok");
  });
  server.on("/release", HTTP_GET, [](AsyncWebServerRequest *request) {
    button.webIsHeld = false;
    request->send(200, "text/plain", "ok");
  });
  server.on("/start", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.print('h');
    request->send(200, "text/plain", "ok");
  });
  server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.print('r');
    request->send(200, "text/plain", "ok");
  });

  server.addHandler(&ws);
  server.begin();
}

void setup() {
  Serial.begin(115200);
  setupLEDOrder();
  pinSetup();
  serverSetup();
}

void loop() {
  uint32_t now = millis();
  handleButtonInput(now);
  checkWebCommand();
  

  if (button.webIsHeld || button.hardIsHeld || button.serialIsHeld) {
    lightNextLED();
  } else {
    turnOffLEDs();
  }
}
