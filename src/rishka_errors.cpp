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

#include <rishka_errors.h>

void rishka_perror(const char* msg, uintptr_t len, bool flush) {
    if(len > 0 && msg)
        for(int i = 0; i < len; i++)
            Serial.write(msg[i]);

    if(flush) {
        Serial.println();
        Serial.flush();
    }
}

void rishka_panic(const char* message) {
    size_t len = strlen(message);

    if(len > 0)
        rishka_perror(message, len, true);

    while(true)
        vTaskDelay(10);
}