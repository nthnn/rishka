/* 
 * This file is part of the Rishka distribution (https://github.com/rishka-esp32/rishka).
 * Copyright (c) 2024 Nathanne Isip.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * This example initializes a TFT display using FabGL library, loads a font for a terminal,
 * initializes an SD card using SPI, initializes a Rishka virtual machine, loads the specified
 * file into the Rishka VM, executes it, and then waits for the next input.
 */

#include <fabgl.h>
#include <rishka.h>
#include <SD.h>
#include <SPI.h>

#define TFT_SCK    18           // TFT SPI clock pin
#define TFT_MOSI   23           // TFT SPI MOSI pin
#define TFT_DC     2            // TFT data/command pin
#define TFT_RESET  4            // TFT reset pin
#define TFT_SPIBUS VSPI_HOST    // TFT SPI bus

#define SD_CS      15           // SD card chip select pin
#define SD_SCK     14           // SD card SPI clock pin
#define SD_MOSI    13           // SD card SPI MOSI pin
#define SD_MISO    12           // SD card SPI MISO pin

// TFT display controller and Terminal instance
fabgl::ILI9341Controller DisplayController;
fabgl::Terminal Terminal;

// SPI instance for SD card
SPIClass sdSpi(HSPI);

void setup() {
    Serial.begin(115200);

    // Initialize TFT display
    DisplayController.begin(TFT_SCK, TFT_MOSI, TFT_DC, TFT_RESET, 5, TFT_SPIBUS);
    DisplayController.setResolution("\"TFT_320x240\" 320 240");

    // Initialize terminal
    Terminal.begin(&DisplayController);
    Terminal.loadFont(&fabgl::FONT_8x14);
    Terminal.enableCursor(true);

    // Initialize SD card
    sdSpi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
    if(!SD.begin(SD_CS, sdSpi, 80000000)) {
        Terminal.println("Card \e[94mMount\e[97m Failed");
        return;
    }

    if(!psramInit()) {
        // If PSRAM initialization fails,
        // print error message and halt execution
        Serial.println("\e[94mCannot\e[97m initialize PSRAM.");
        while(true);
    }

    // Print prompt
    Terminal.print("\e[32m#~\e[97m ");
}

void loop() {
    // Check if there is data available to read from serial port
    if(!Serial.available())
        return;

    // Read input from serial port
    String input = Serial.readString();
    // Echo input back to serial port
    Terminal.print("\e[93m");
    Terminal.print(input);
    Terminal.print("\r\e[97m");

    // Rishka virtual machine instance
    RishkaVM* vm = new RishkaVM();
    // Initialize Rishka virtual machine
    vm->initialize(&Terminal);

    // Attempt to load specified file into Rishka virtual machine
    if(!vm->loadFile(input.c_str())) {
        // If loading file fails, print error message and return
        Terminal.print("Failed to \e[94mload\e[97m specified file: " + input);
        Terminal.print("\r\e[32m#~\e[97m ");
        return;
    }

    // Run loaded program on Rishka virtual machine
    vm->run(0, NULL);
    // Reset Rishka virtual machine for next execution
    vm->reset();

    // Delete VM instance.
    delete vm;

    // Print prompt for next input
    Terminal.print("\e[32m#~\e[97m ");
}