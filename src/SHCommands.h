#define MESSAGE_HEADER 0x03


void Command_Hello()
{
	FlowSerialTimedRead();
	delay(10);
	FlowSerialPrint(VERSION);
	FlowSerialFlush();
}

void Command_SetBaudrate()
{
	SetBaudrate();
}

void Command_ButtonsCount()
{
	FlowSerialWrite((byte)(ENABLED_BUTTONS_COUNT));
	FlowSerialFlush();
}

void Command_TM1638Count()
{
	FlowSerialWrite((byte)(0));
	FlowSerialFlush();
}

void Command_SimpleModulesCount()
{
	FlowSerialWrite((byte)(0));
	FlowSerialFlush();
}

void Command_Acq()
{
	FlowSerialWrite(0x03);
	FlowSerialFlush();
}

void Command_DeviceName()
{
	FlowSerialPrint(DEVICE_NAME);
	FlowSerialPrint("\n");
	FlowSerialFlush();
}

void Command_UniqueId()
{
	auto id = getUniqueId();
	FlowSerialPrint(id);
	FlowSerialPrint("\n");
	FlowSerialFlush();
}

void Command_MCUType()
{
	FlowSerialPrint(SIGNATURE_0);
	FlowSerialPrint(SIGNATURE_1);
	FlowSerialPrint(SIGNATURE_2);
	FlowSerialFlush();
}


void Command_GLCDData()
{
	screen.read();
}

void Command_ExpandedCommandsList()
{
	FlowSerialPrintLn("mcutype");
	FlowSerialPrintLn("keepalive");
	FlowSerialPrintLn();
	FlowSerialFlush();
}

void Command_Features()
{
	delay(10);

	FlowSerialPrint("K"); // screens

	// Name
	FlowSerialPrint("N");

	// UniqueID
	FlowSerialPrint("I");

	// Xpanded support
	FlowSerialPrint("X");

	FlowSerialPrint("\n");
	FlowSerialFlush();
}

void Command_RGBLEDSCount()
{
	FlowSerialWrite((byte)(WS2812B_RGBLEDCOUNT));
	FlowSerialFlush();
}

void Command_RGBLEDSData()
{
	shRGBLedsWS2812B.read();

	// Acq !
	FlowSerialWrite(0x15);
}

void Command_RGBMatrixData()
{
	// Acq !
	FlowSerialWrite(0x15);
}

void Command_GearData()
{
	char gear = FlowSerialTimedRead();
}

void Command_CustomProtocolData()
{
	FlowSerialWrite(0x15);
}
