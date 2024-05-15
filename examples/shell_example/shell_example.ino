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

#define TFT_CS     5            // TFT SPI select pin
#define TFT_SCK    18           // TFT SPI clock pin
#define TFT_MOSI   23           // TFT SPI MOSI pin
#define TFT_DC     15           // TFT data/command pin
#define TFT_RESET  4            // TFT reset pin
#define TFT_SPIBUS VSPI_HOST    // TFT SPI bus

#define SD_CS      2            // SD card chip select pin
#define SD_SCK     14           // SD card SPI clock pin
#define SD_MOSI    13           // SD card SPI MOSI pin
#define SD_MISO    12           // SD card SPI MISO pin

// TFT display controller, PS/2 keyboard controller,
// and Terminal instance
fabgl::ILI9341Controller DisplayController;
fabgl::PS2Controller     PS2Controller;
fabgl::Terminal          Terminal;

// SPI instance for SD card
SPIClass sdSpi(HSPI);

// Rishka virtual machine instance
RishkaVM* vm;

// This function utilizes a LineEditor instance to read a line
// from the terminal input. It then returns the read line as a String.
static inline String readLine() {
    fabgl::LineEditor line(&Terminal);
    line.edit();

    return String(line.get());
}

// This function splits a given input string into tokens based on space characters.
// It also handles quotes to allow space characters within quoted strings.
void splitString(const String& input, char** tokens, int maxTokens, int &count) {
    int tokenCount = 0, tokenStart = 0;
    bool inQuotes = false;

    // Iterate over each character in the input string
    for(int i = 0; i < input.length(); i++) {
        // Check if the current character is a quotation mark
        if(input[i] == '"')
            // Toggle the inQuotes flag to handle quoted strings
            inQuotes = !inQuotes;
        else if(input[i] == ' ' && !inQuotes) {
            // If the current character is a space and not inside quotes,
            // extract the token from the input string
            if(tokenCount < maxTokens) {
                // Extract the substring between tokenStart and i
                // and copy it into the tokens array
                input.substring(tokenStart, i)
                    .toCharArray(tokens[tokenCount], i - tokenStart + 1);

                // Null-terminate the token string
                tokens[tokenCount++][i - tokenStart] = '\0';
                tokenStart = i + 1;
                count++;
            }
            else break; // Break if maximum token count reached
        }
    }

    // Handle the case where there are remaining characters after the last space
    if(tokenCount < maxTokens && tokenStart < input.length()) {
        // Extract the substring starting from tokenStart to the end of the input string
        // and copy it into the tokens array
        input.substring(tokenStart).toCharArray(tokens[tokenCount], input.length() - tokenStart + 1);
        tokens[tokenCount++][input.length() - tokenStart] = '\0';
        count++;
    }
}

void setup() {
    Serial.begin(115200);
    PS2Controller.begin(PS2Preset::KeyboardPort0);

    // Initialize TFT display
    DisplayController.begin(TFT_SCK, TFT_MOSI, TFT_DC, TFT_RESET, TFT_CS, TFT_SPIBUS);
    DisplayController.setResolution("\"TFT_320x240\" 320 240");

    // Initialize terminal
    Terminal.begin(&DisplayController);
    Terminal.loadFont(&fabgl::FONT_8x14);
    Terminal.setBackgroundColor(Color::Black);
    Terminal.setForegroundColor(Color::White);
    Terminal.connectLocally();
    Terminal.clear();
    Terminal.enableCursor(true);

    // Initialize SD card
    sdSpi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
    if(!SD.begin(SD_CS, sdSpi, 80000000)) {
        // If PSRAM initialization fails,
        // print error message and halt execution
        Terminal.println("Card \e[94mMount\e[97m Failed");

        // Wait for SD card.
        do sdSpi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
        while(!SD.begin(SD_CS, sdSpi, 80000000));

        // Clear terminal error message
        Terminal.clear();
    }

    if(!psramInit()) {
        Terminal.println("\e[94mCannot\e[97m initialize PSRAM.");
        while(true);
    }

    // Initialize the Rishka VM instance.
    vm = new RishkaVM();
    vm->initialize(&Terminal);

    // Virtual key listener to halt program
    Terminal.onVirtualKeyItem = [&](VirtualKeyItem * vkItem) {
        if(vkItem->CTRL && vkItem->vk == VirtualKey::VK_c && vm->isRunning()) {
            // Stop the VM if CTRL+C was pressed
            vm->stopVM();
            vm->reset();

            Terminal.println("^C");
        }
    };

    // Print prompt
    Terminal.print("\e[32m[\e[97m" + vm->getWorkingDirectory() + "\e[97m\e[32m]~\e[97m ");
}

void loop() {
    // Read input from PS/2 keyboard
    String input = readLine();

    // Skip if input is empty
    if(input == "") {
        // Print prompt
        Terminal.print(String("\e[32m[\e[97m") +
            vm->getWorkingDirectory() + "\e[97m\e[32m]~\e[97m ");
        return;
    }

    char* tokens[10];
    int count = 0;

    // Split input string as argument tokens
    for(int i = 0; i < 10; i++)
        tokens[i] = (char*) malloc(50 * sizeof(char));
    splitString(input, tokens, 10, count);

    // Attempt to load specified file into Rishka virtual machine
    if(!vm->loadFile(tokens[0])) {
        // If loading file fails, print error message and return
        Terminal.println("Failed to \e[94mload\e[97m specified file: " +
          String(tokens[0]));

        // Free all tokens in memory
        for(int i = 0; i < 10; i++)
            free(tokens[i]);

        // Reset Rishka virtual machine for next execution
        vm->reset();

        // Print prompt
        Terminal.print(String("\e[32m[\e[97m") +
            vm->getWorkingDirectory() + "\e[97m\e[32m]~\e[97m ");
        return;
    }

    // Run loaded program on Rishka virtual machine
    vm->run(count, tokens);
    // Reset Rishka virtual machine for next execution
    vm->reset();

    // Free all tokens in memory
    for(int i = 0; i < 10; i++)
        free(tokens[i]);

    // Print prompt for next input
    Terminal.print(String("\e[32m[\e[97m") +
        vm->getWorkingDirectory() + "\e[97m\e[32m]~\e[97m ");
}
