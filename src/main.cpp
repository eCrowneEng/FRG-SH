#include <EspSimHub.h>

// Plus is just optional code that may add extra functionality, currently not included, but you can provide
//  your own implementation and hook into the same callsites, just provide a PlusController in a header file
#define PLUS 0

// J revision sketch
#define VERSION 'j'

#define DEVICE_NAME "FlexRingGauge"     // Device name, make sure to use a unique name when using multiple devices
#define WS2812B_RGBLEDCOUNT 24       	// WS2812B RGB leds count
#define WS2812B_DATAPIN D6              // Data (DIN) digital pin number for RGB LED Ring
#define WS2812B_RGBENCODING 0        	// WS2812B RGB encoding, Set to 0 for GRB, 1 for RGB encoding, 2 for BRG encoding
#define WS2812B_RIGHTTOLEFT 0        	// Reverse led order
#define LEDRING_SHIFT_BY_180_DEGREES 1  // If set to 1, uses an alternative mapping for leds that's shifted 180 degrees
#define FASTLED_ESP8266_RAW_PIN_ORDER	// fastLED will try to guess your pin number if not set up and won't work
#define LED_BUILTIN D4					// Pin number to activate to control the built in LED

/** *****************************************
 * 
 * Be careful editing the code below
 * 
 **************************************** **/

#include <EEPROM.h>
#include <SPI.h>
#include <Arduino.h>
#include <Wire.h>
#include <FlowSerialRead.h>
#include <SHRGBLedsNeoPixelFastLed.h>

#if PLUS > 0
# include <plus.h>
#endif

char loop_opt;
unsigned long lastSerialActivity = 0;

#include <Adafruit_GFX.h>
#include <SHGLCD_I2COLED.h>
SHGLCD_I2COLED screen;
SHRGBLedsNeoPixelFastLeds shRGBLedsWS2812B;

#if PLUS > 0
PlusController plus(&screen, &shRGBLedsWS2812B);
#endif

#include <SHCommands.h>

void idle(bool critical) {}

void setup()
{
	FlowSerialBegin(19200);
	shRGBLedsWS2812B.begin(WS2812B_RGBLEDCOUNT, WS2812B_RIGHTTOLEFT, PLUS == 0);
	screen.init(PLUS == 0);
	arqserial.setIdleFunction(idle);
	#if PLUS > 0
		plus.init();
	#endif

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
			lastSerialActivity = millis();
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
	} else if (lastSerialActivity == 0) {
		// serial port is not connected
		#if PLUS > 0
			plus.idleLoop();
		#endif
	}
}
