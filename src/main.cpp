#include <EspSimHub.h>

// J revision sketch
#define VERSION 'j'

// Device name, make sure to use a unique name when using multiple devices
#define DEVICE_NAME "FlexRingGauge"
// A unique id between your devices
//  get a new one from https://www.uuidgenerator.net/
#define DEVICE_UNIQUE_ID "00000000-0000-0000-0000-000000000000"
// How many leds in the ring
#define WS2812B_RGBLEDCOUNT 24
// Ring data pin
#define WS2812B_DATAPIN D6
// Encoding; 0 is GRB, 1 is RGB, 2 is BRG; 
//  Unless you have a good reason to change this, leave it as 0
#define WS2812B_RGBENCODING 0
// Reverse led order: 0 is false
#define WS2812B_RIGHTTOLEFT 0
// Enable showing when the gauge is not connected, it clears when connected
#define WS2812B_TESTMODE 1
// If set to 1, uses an alternative mapping for leds that's shifted 180 degrees
#define LEDRING_SHIFT_BY_180_DEGREES 1
// fastLED will try to guess your pin definitions, if not set up and won't work
//  it may remap pins and make stuff not work if this is not set
#define FASTLED_ESP8266_RAW_PIN_ORDER
// Built in LED pin
#define LED_BUILTIN D4

/** *****************************************
 * 
 * Be careful editing the code below
 * 
 **************************************** **/

#include <EEPROM.h>
#include <SPI.h>
#include "Arduino.h"
#include <Wire.h>
#include <FlowSerialRead.h>
#include <SHRGBLedsNeoPixelFastLed.h>

char loop_opt;

#include "Adafruit_GFX.h"
#include "SHGLCD_I2COLED.h"
SHGLCD_I2COLED screen;
SHRGBLedsNeoPixelFastLeds shRGBLedsWS2812B;

#include "SHCommands.h"

void idle(bool critical) {}

void setup()
{
	FlowSerialBegin(19200);
	shRGBLedsWS2812B.begin(WS2812B_RGBLEDCOUNT, WS2812B_RIGHTTOLEFT, WS2812B_TESTMODE);
	screen.init();
	arqserial.setIdleFunction(idle);

	// There is no use for the built in led yet, but this is how you turn it on
	// pinMode(LED_BUILTIN, OUTPUT);
	// digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{

	// Wait for data
	if (FlowSerialAvailable() > 0)
	{
		if (FlowSerialTimedRead() == MESSAGE_HEADER)
		{
			// Read command
			loop_opt = FlowSerialTimedRead();

			if (loop_opt == '1')
				Command_Hello();
			else if (loop_opt == '8')
				Command_SetBaudrate();
			else if (loop_opt == 'J')
				Command_ButtonsCount();
			else if (loop_opt == '2')
				Command_TM1638Count();
			else if (loop_opt == 'B')
				Command_SimpleModulesCount();
			else if (loop_opt == 'A')
				Command_Acq();
			else if (loop_opt == 'N')
				Command_DeviceName();
			else if (loop_opt == 'I')
				Command_UniqueId();
			else if (loop_opt == '0')
				Command_Features();
			else if (loop_opt == '4')
				Command_RGBLEDSCount();
			else if (loop_opt == '6')
				Command_RGBLEDSData();
			else if (loop_opt == 'R')
				Command_RGBMatrixData();
			else if (loop_opt == 'G')
				Command_GearData();
			else if (loop_opt == 'K')
				Command_GLCDData(); // Nokia | OLEDS
			else if (loop_opt == 'P')
				Command_CustomProtocolData();
			else if (loop_opt == 'X')
			{
				String xaction = FlowSerialReadStringUntil(' ', '\n');
				if (xaction == F("list"))
					Command_ExpandedCommandsList();
				else if (xaction == F("mcutype"))
					Command_MCUType();
			}
		}
	}
}
