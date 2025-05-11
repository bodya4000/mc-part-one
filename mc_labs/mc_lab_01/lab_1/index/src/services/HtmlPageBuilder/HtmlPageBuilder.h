#pragma once
#include <Arduino.h>

class HtmlPageBuilder {
public:
    String home();
    String notFound();
		String homeWithWebSocket();

};
