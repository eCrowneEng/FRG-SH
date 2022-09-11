#ifndef __SHRGBLEDSNEOPIXELFASTLED_H__
#define __SHRGBLEDSNEOPIXELFASTLED_H__
#define FASTLED_ALLOW_INTERRUPTS 0


#include <Arduino.h>
#include "SHRGBLedsBase.h"
#include <FastLED.h>

CRGB SHRGBLedsNeoPixelFastLeds_leds[WS2812B_RGBLEDCOUNT];
#if LEDRING_SHIFT_BY_180_DEGREES > 0
byte remapping[24] = {12,13,14,15,16,17,18,19,20,21,22,23,0,1,2,3,4,5,6,7,8,9,10,11};
#else
byte remapping[24] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
#endif


class SHRGBLedsNeoPixelFastLeds : public SHRGBLedsBase {
private:
	unsigned long lastRead = 0;
public:

	void begin(int maxLeds, int righttoleft, bool testMode) {
		SHRGBLedsBase::begin(maxLeds, righttoleft);
		FastLED.setCorrection(TypicalLEDStrip);
		FastLED.setMaxPowerInVoltsAndMilliamps(5, 450);
		FastLED.addLeds<NEOPIXEL, WS2812B_DATAPIN>(SHRGBLedsNeoPixelFastLeds_leds, maxLeds);

		if (testMode > 0) {
			for (int i = 0; i < maxLeds; i++) {
				setPixelColor(i, 10, 0, 0);
			}
		}
		FastLED.show();
		FastLED.show(); // needs duplicate in the ESP boards for timing purposes
	}

	void show() {
		FastLED.show();
	}

protected:
	void setPixelColor(uint8_t lednumber, uint8_t r, uint8_t g, uint8_t b) {
		// 0 for GRB, 
		// 1 for RGB encoding
		// 2 for BRG encoding
		if (WS2812B_RGBENCODING == 0) {
			SHRGBLedsNeoPixelFastLeds_leds[remapping[lednumber]].setRGB(r, g, b);
		}
		else if (WS2812B_RGBENCODING == 1) {
			SHRGBLedsNeoPixelFastLeds_leds[remapping[lednumber]].setRGB(g, r, b);
		}
		else if (WS2812B_RGBENCODING == 2) {
			SHRGBLedsNeoPixelFastLeds_leds[remapping[lednumber]].setRGB(b, g, r);
		}
	}
};

#endif