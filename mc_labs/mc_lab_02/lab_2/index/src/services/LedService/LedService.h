#pragma once
#include <vector>
#include "../../hardware/Led/Led.h"
#include "../../hardware/Led/Color.h"

class LedService {
	private:
		std::vector<Led*> leds;
		uint8_t currentIndex = 0;
		bool isRunning = false;
	
	public:
		LedService(std::vector<Led*> leds);

		bool getIsRunning() const;
		void go();
		void stop();
	};
	
