/* 
 * This file is part of the Rishka distribution (https://github.com/rishka-esp32/rishka-sdk).
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

void create_file();
void read_file();
void delete_file();

i32 main() {
    create_file();
    read_file();
    delete_file();

    return 0;
}

void create_file() {
    File file = File::open(F("/test.txt"), F("w"));

    IO::print(F("Writing to file: "));
    IO::print(file.name());
    IO::print(F("\r\n"));

    file.write(F("Hello, world!"));
    file.close();
}

void read_file() {
    File file = File::open(F("/test.txt"), F("r"));

    string contents;
    Memory::set(contents, 0, file.size() + 1);

    IO::print(F("Reading from file: "));
    IO::print(file.name());
    IO::print(F("\r\n"));

    i32 data, idx = 0;
    while((data = file.read()) != -1)
        contents[idx++] = (rune) data;

    contents[idx] = '\0';
    file.close();

    IO::print(F("Contents: "));
    IO::print(contents);
    IO::print(F("\r\n"));
}

void delete_file() {
    if(FS::remove(F("/test.txt")))
        IO::print(F("Deleted /test.txt\r\n"));
    else IO::print(F("Can't delete /test.txt\r\n"));
}