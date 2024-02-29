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

#ifndef LIBRISHKA_FS_H
#define LIBRISHKA_FS_H

#include <librishka/types.h>

class File final {
private:
    i32 handle;

protected:
    File(i32 _handle);

public:
    static File open(string file, string mode);

    bool is_file();
    bool is_dir();

    i32 available();
    i32 peek();
    u64 lastwrite();
    bool bufsize(usize size);
    bool seek(u32 pos);
    bool seek_dir(u64 position);

    usize size();
    usize position();

    i32 read();
    void write(u8 data);
    void write(string data);

    string path();
    string name();

    File next(string mode);
    string next_name();

    void flush();
    void close();
    void rewind();
};

class FS final {
public:
    static bool mkdir(const char* path);
    static bool rmdir(const char* path);
    static bool remove(const char* path);
    static bool exists(const char* path);
};

#endif