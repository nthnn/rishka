/* 
 * This file is part of the Rishka distribution (https://github.com/nthnn/rishka).
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

#include <librishka.h>

#define LED 2

i32 main() {
    Gpio::pin_mode(LED, GPIO_OUTPUT);

    for(i8 i = 0; i < 3; i++) {
        Gpio::digital_write(LED, GPIO_HIGH);
        Sys::delay(1000);

        Gpio::digital_write(LED, GPIO_LOW);
        Sys::delay(1000);
    }

    return 0;
}