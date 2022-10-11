#ifndef __SHGLCD_BASE_H__
#define __SHGLCD_BASE_H__

#include <Arduino.h>
#include <Adafruit_GFX.h>

#include <Open24DisplaySt12pt7b.h>
#define CUSTOM_LCD_FONT_1 Open24DisplaySt12pt7b

#include <Open24DisplaySt18pt7b.h>
#define CUSTOM_LCD_FONT_2 Open24DisplaySt18pt7b

class SHGLCD_Base
{
private:
	//uint8_t  fontSize;
	int16_t  posX;
	int16_t  posY;
	char action;
	int16_t v[4 * 2];
	uint16_t color;
	int16_t w;
	int16_t h;
	int16_t r;
	uint8_t align = 0;
	//boolean wrap = 0;

	int16_t boundX;
	int16_t boundY;
	uint16_t boundW;
	uint16_t boundH;

	uint8_t nokiaIndex;
	uint8_t fontType;

	Adafruit_GFX* currentNokia;

public:

	virtual void init(bool animate) = 0;

	virtual void Display(int idx) = 0;

	virtual void ClearDisplay(int idx) = 0;

	virtual void SetContrast(int idx, int c) = 0;

	virtual int GetScreenCount() = 0;

	virtual Adafruit_GFX * GetScreen(int idx) = 0;

	void read() {
		nokiaIndex = FlowSerialTimedRead();
		action = FlowSerialTimedRead();

		currentNokia = GetScreen(nokiaIndex);

		// clear the screen
		if (action == 'C')
		{
			ClearDisplay(nokiaIndex);
		}

		// feature request (Nokia)
		if (action == 'N')
		{
			FlowSerialWrite((byte) 0);
			FlowSerialFlush();
		}

		// feature request (OLED)
		if (action == 'O')
		{
			FlowSerialWrite((byte) 1);
			FlowSerialFlush();
		}

		// Set contrast
		if (action == 'I')
		{
			int c = FlowSerialTimedRead();
			SetContrast(nokiaIndex, c);
		}

		// Push data in ram to display
		else if (action == 'D')
		{
			Display(nokiaIndex);
		}

		// print text
		else if (action == 'P')
		{
			currentNokia->setTextSize((uint8_t)FlowSerialTimedRead());
			fontType = (uint8_t)FlowSerialTimedRead();
			posX = (int16_t)FlowSerialTimedRead();
			posY = (int16_t)FlowSerialTimedRead();
			currentNokia->setTextColor(FlowSerialTimedRead());
			currentNokia->setTextWrap(FlowSerialTimedRead() > 0);
			align = FlowSerialTimedRead();

			String content = FlowSerialReadStringUntil('\n');

			currentNokia->setFont();

			if (fontType == 1) {
				currentNokia->setFont(&CUSTOM_LCD_FONT_1);
			}
			else if (fontType == 2) {
				currentNokia->setFont(&CUSTOM_LCD_FONT_2);
			}

			if (align == 2)
			{
				String content2 = content + "\n ";
				currentNokia->getTextBounds((char*)content2.c_str(), 0, 0, &boundX, &boundY, &boundW, &boundH);
				posX = posX - (boundW / 2);
			}
			else if (align == 3)
			{
				String content2 = content + "\n ";
				currentNokia->getTextBounds((char*)content2.c_str(), 0, 0, &boundX, &boundY, &boundW, &boundH);
				posX = posX - boundW;
			}

			currentNokia->setCursor(posX, posY);
			currentNokia->print(content);
		}

		// Draw a line
		else if (action == 'L') {
			posX = (int16_t)FlowSerialTimedRead();
			posY = (int16_t)FlowSerialTimedRead();
			w = (int16_t)FlowSerialTimedRead(); // x2
			h = (int16_t)FlowSerialTimedRead(); // y2
			color = FlowSerialTimedRead();

			currentNokia->drawLine(posX, posY, w, h, color);
		}

		// Draw a triangle
		else if (action == 'T') {
			posX = (int16_t)FlowSerialTimedRead();
			posY = (int16_t)FlowSerialTimedRead();
			w = (int16_t)FlowSerialTimedRead(); // x2
			h = (int16_t)FlowSerialTimedRead(); // y2
			int thickness = FlowSerialTimedRead();
			color = FlowSerialTimedRead();

			float dx = posX - w;
			float dy = posY - h;
			float d = sqrtf(dx * dx + dy * dy);
			if (!d)
				return;

			int v[4 * 2];

			/* left up */
			v[0] = posX - thickness * dy / d;
			v[1] = posY + thickness * dx / d;
			/* right up */
			v[2] = posX + thickness * dy / d;
			v[3] = posY - thickness * dx / d;
			/* right down */
			v[4] = w + thickness * dy / d;
			v[5] = h - thickness * dx / d;
			/* left down */
			v[6] = w - thickness * dy / d;
			v[7] = h + thickness * dx / d;

			currentNokia->fillTriangle(v[0], v[1], v[6], v[7], v[2], v[3], color);
			currentNokia->fillTriangle(v[2], v[3], v[4], v[5], v[6], v[7], color);
		}

		// Draw rectangle, either rounded or normal; filled or outlined
		else if (action == 'F' || action == 'R')
		{
			posX = (int16_t)FlowSerialTimedRead();
			posY = (int16_t)FlowSerialTimedRead();
			w = (int16_t)FlowSerialTimedRead();
			h = (int16_t)FlowSerialTimedRead();
			r = (int16_t)FlowSerialTimedRead();
			color = FlowSerialTimedRead();

			if (r == 0) {
				if (action == 'F')
					currentNokia->fillRect(posX, posY, w, h, color);
				else
					currentNokia->drawRect(posX, posY, w, h, color);
			}
			else {
				if (action == 'F')
					currentNokia->fillRoundRect(posX, posY, w, h, r, color);
				else
					currentNokia->drawRoundRect(posX, posY, w, h, r, color);
			}
		}
	}
};
#endif