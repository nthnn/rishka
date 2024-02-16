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

#include <rishka.h>

void setup() {
    Serial.begin(115200);
    while(!Serial);

    if(!SD.begin(5)) {
        Serial.println("Failed to initialize SD card.");
        while(true);
    }

    if(!rishka_vm_loadfile(&riscvm_machine, "/hello.bin"))
        rishka_panic("Failed to load specified file.");

    rishka_vm_run(&riscvm_machine, 0, NULL);
    rishka_vm_reset(&riscvm_machine);
}

void loop() {
    vTaskDelay(10);
}