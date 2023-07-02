# FRG-SH
FlexRingGauge firwmare for using it with SimHub. 

For the full project join the Discord:

https://discord.gg/EBbzWWyGgr - Build instructions, STLs, etc.

## TL:DR; How do I upload this?
- Download a [release](https://github.com/marcelstoer/nodemcu-pyflasher/releases) from here [NodeMCU PyFlasher](https://github.com/marcelstoer/nodemcu-pyflasher)
- Download the latest firmware for the [FRG-SH](https://github.com/eCrowneEng/FRG-SH/releases)
- Connect the Meter
- Open NodeMCU PyFlasher. Pick the serial port of the device, select the firmware, Baud Rate: 115200, Flash mode: Dual I/O, Erase Flash: no. Flash NodeMCU.

## About
FlexRingGauge is a device that aims to be a meter with a display and a configurable sweep to display measurements.

While in theory the device is not tied to SimHub, this firmware emulates an Arduino board that has been flashed with the SimHub firmware.

## Required components
- 24 RGB LED Ring - aka "NeoPixel"
- 1.3" OLED Screen with the SSH1106 driver that can be wired for I2C (usually these have 4 pins only)
- D1 Mini (ESP8266)

The ESP866 was selected over any Arduino board due to cost, form factor available memory AND performance. In the past I tested a similar meter stand alone framework and the affordable Arduino boards struggled to keep up, while the ESP8266 was passed with flying colors.
The SSH1106 screen was picked due to its availability, size and cost.
Given that SimHub doesn't support neither the ESP board nor the SSH1106 driver for the screen, this repository exists.

## Circuit
This firmware assumes that the RGB ring is connected to `D6` pin of the D1 Mini board.
The screen's `SCL` connects to `D1` and `SDA` to `D2`

<img src="https://raw.githubusercontent.com/eCrowneEng/FRG-SH/main/diagram.png" width="1000" />

## Code
The source code for the firmware is meant to be opened with [Platformio (VS Code + Platformio Plugin)](https://platformio.org/platformio-ide) rather than Arduino IDE. It also means that the entry point for the code is in `src/main.cpp` rather than an `.ino` file, but don't worry, it's the same on the inside.

The reason for this is because [Platformio](https://platformio.org/) is a much better editor, dependency management is superior, and many more reasons. [Here's a good and very long read about it](https://nerdytechy.com/platformio-vscode-vs-arduino-ide/).

However I won't force you to use it if you just want to upload the firwmare, Download the `.bin` file you need from the [release section](https://github.com/eCrowneEng/FRG-SH/releases).
See the TL:DR; section above with the suggested method of upload. It should take you 5 minutes.


