#ifndef __SHGLCD_I2COLED_H__
#define __SHGLCD_I2COLED_H__

#include <Arduino.h>
#include "Adafruit_GFX.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_SH110X.h>
#include "SHGLCD_base.h"

#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's

#define OLED_RESET -1

Adafruit_SH1106G glcd1(128, 64, &Wire, OLED_RESET);
Adafruit_SH1106G * oled[] = { &glcd1 };

class SHGLCD_I2COLED : public SHGLCD_Base
{
public:

	void init(bool animate) {
		glcd1.begin(i2c_Address, true);
		glcd1.clearDisplay();
		if (animate) {
			glcd1.setFont();
			glcd1.setTextSize(2);
			glcd1.setTextColor(SH110X_WHITE);
			glcd1.setCursor(15, 15);
			glcd1.print("FlexRing");
			glcd1.setCursor(35, 35);
			glcd1.print("Gauge");
			glcd1.display();
		}
	}

	void Display(int idx) {
		oled[idx]->display();
	}

	void ClearDisplay(int idx) {
		oled[idx]->clearDisplay();
	}

	void SetContrast(int idx, int c) {}

	int GetScreenCount() {
		return 1;
	}

	Adafruit_GFX * GetScreen(int idx) {
		return oled[idx];
	}

};
#endif