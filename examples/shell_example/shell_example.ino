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
 * This example demonstrates the usage of Rishka virtual machine
 * on an ESP32-WROVER microcontroller. It initializes serial communication
 * and SD card, waits for user input via serial port, loads the specified
 * file into the Rishka VM, executes it, and then waits for the next input.
 */

#include <rishka.h>

void setup() {
    // Begin serial communication at 115200 baud rate
    Serial.begin(115200);
    // Wait until serial connection is established
    while(!Serial);

    // Initialize SD card on pin 5
    if(!SD.begin(5)) {
        // If SD card initialization fails, 
        // print error message and halt execution
        Serial.println("Failed to initialize SD card.");
        while(true);
    }

    // Initialize Rishka virtual machine
    rishka_vm_initialize(&rishka_vm);
    // Print prompt
    Serial.print("> ");
}

void loop() {
    // Check if there is data available to read from serial port
    if(!Serial.available())
        return;

    // Read input from serial port
    String input = Serial.readString();
    // Echo input back to serial port
    Serial.print(input);

    // Attempt to load specified file into Rishka virtual machine
    if(!rishka_vm_loadfile(&rishka_vm, input.c_str())) {
        // If loading file fails, print error message and return
        Serial.println("Failed to load specified file: " + input);
        return;
    }

    // Run loaded program on Rishka virtual machine
    rishka_vm_run(&rishka_vm, 0, NULL);
    // Reset Rishka virtual machine for next execution
    rishka_vm_reset(&rishka_vm);

    // Print prompt for next input
    Serial.print("> ");
}