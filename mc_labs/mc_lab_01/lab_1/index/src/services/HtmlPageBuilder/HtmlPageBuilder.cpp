#include <Arduino.h>
#include "HtmlPageBuilder.h"

String HtmlPageBuilder::home() {
    return R"rawliteral(
        <!DOCTYPE html>
					<html>
						<head>
							<title>LAB_1</title>
						</head>
						<body style="text-align: center; padding-top: 50px">
							<button id="Button">🖱️ Hold Me</button>

							<script>
								let holdTimeout;
								const btn = document.getElementById('Button');

								btn.addEventListener('mousedown', () => {
									holdTimeout = setTimeout(() => {
										fetch('/press', { method: 'POST' });
									}, 1000);
								});

								btn.addEventListener('mouseup', () => {
									clearTimeout(holdTimeout);
									fetch('/press_out', { method: 'POST' });
								});
							</script>
						</body>
					</html>

    )rawliteral";
}

String HtmlPageBuilder::homeWithWebSocket() {
	return R"rawliteral(
		<!DOCTYPE html>
		<html>
		<head>
			<title>LAB_1 WebSocket</title>
			<style>
				body {
					text-align: center;
					padding-top: 50px;
					background: #f0f4f8;
					font-family: Arial, sans-serif;
				}

				#Button {
					font-size: 24px;
					padding: 20px 50px;
					border: none;
					border-radius: 12px;
					background: linear-gradient(145deg, #6c63ff, #42a5f5);
					color: white;
					cursor: pointer;
					box-shadow: 0 8px 15px rgba(0, 0, 0, 0.2);
					transition: all 0.3s ease;
					position: relative;
					overflow: hidden;
				}

				#Button::after {
					content: "";
					position: absolute;
					top: 0;
					left: -100%;
					width: 100%;
					height: 100%;
					background: rgba(255, 255, 255, 0.3);
					transition: left 0.4s ease;
				}

				#Button:hover::after {
					left: 100%;
				}

				#Button:hover {
					transform: scale(1.05);
					box-shadow: 0 10px 20px rgba(0, 0, 0, 0.3);
				}

				#Button:active {
					transform: scale(0.98);
				}
			</style>
		</head>
		<body>
			<button id="Button">🖱️ Hold Me</button>

			<script>
				const socket = new WebSocket(`ws://${location.host}/ws`);
				const btn = document.getElementById('Button');
				let holdTimeout;

				socket.onopen = () => {
					console.log("WebSocket connected");
				};

				socket.onerror = (error) => {
					console.error("WebSocket error:", error);
				};

				socket.onmessage = (event) => {
					console.log("Server:", event.data);
				};

				btn.addEventListener('mousedown', () => {
					holdTimeout = setTimeout(() => {
						socket.send('press');
					}, 1000);
				});

				btn.addEventListener('mouseup', () => {
					clearTimeout(holdTimeout);
					socket.send('release');
				});
			</script>
		</body>
		</html>
	)rawliteral";
}


String HtmlPageBuilder::notFound() {
    return "<h1>404 - Not found</h1>";
}
