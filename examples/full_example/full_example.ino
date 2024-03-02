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
 * to load and execute a specific binary file ("/hello.bin") stored
 * on an SD card. It initializes serial communication, SD card,
 * Rishka VM, loads the binary file, executes it, and then resets
 * the VM. After setup, it enters an idle loop.
 */

#include <rishka.h>

// Create a Rishka VM
rishka_virtual_machine vm;

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

    if(!psramInit()) {
        // If PSRAM initialization fails,
        // print error message and halt execution
        Serial.println("Cannot initialize PSRAM.");
        while(true);
    }

    // Initialize Rishka virtual machine
    rishka_vm_initialize(&vm, &Serial);

    // Attempt to load specified file ("/hello.bin")
    // into Rishka virtual machine
    if(!rishka_vm_loadfile(&vm, "/hello.bin"))
        // If loading file fails, print error
        // message and halt execution
        rishka_panic("Failed to load specified file.", &vm);

    // Run loaded program on Rishka virtual machine
    rishka_vm_run(&vm, 0, NULL);
    // Reset Rishka virtual machine for next execution
    rishka_vm_reset(&vm);
}

void loop() {
    // Delay execution to avoid busy-waiting
    vTaskDelay(10);
}