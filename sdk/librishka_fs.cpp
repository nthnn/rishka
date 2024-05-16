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

#include "librishka.h"
#include "librishka_impl.hpp"

File::File(i32 _handle) {
    this->handle = _handle;
}

File File::open(string file, string mode) {
    return File((i32) rishka_sc_2(RISHKA_SC_FS_OPEN, (i64) file, (i64) mode));
}

bool File::is_file() {
    return (bool) rishka_sc_1(RISHKA_SC_FS_ISFILE, (i64) this->handle);
}

bool File::is_dir() {
    return (bool) rishka_sc_1(RISHKA_SC_FS_ISDIR, (i64) this->handle);
}

i32 File::available() {
    return (i32) rishka_sc_1(RISHKA_SC_FS_AVAILABLE, (i64) this->handle);
}

i32 File::peek() {
    return (i32) rishka_sc_1(RISHKA_SC_FS_PEEK, (i64) this->handle);
}

bool File::seek(u32 pos) {
    return (bool) rishka_sc_2(RISHKA_SC_FS_SEEK, (i64) this->handle, (i64) pos);
}

usize File::size() {
    return (usize) rishka_sc_1(RISHKA_SC_FS_SIZE, (i64) this->handle);
}

usize File::position() {
    return (usize) rishka_sc_1(RISHKA_SC_FS_POS, (i64) this->handle);
}

i32 File::read() {
    return (i32) rishka_sc_1(RISHKA_SC_FS_READ, (i64) this->handle);
}

void File::write(u8 data) {
    rishka_sc_2(RISHKA_SC_FS_WRITEB, (i64) this->handle, (i64) data);
}

void File::write(string data) {
    rishka_sc_2(RISHKA_SC_FS_WRITES, (i64) this->handle, (i64) data);
}

string File::path() {
    return get_rt_string(rishka_sc_1(RISHKA_SC_FS_PATH, (i64) this->handle));
}

string File::name() {
    return get_rt_string(rishka_sc_1(RISHKA_SC_FS_NAME, (i64) this->handle));
}

File File::next() {
    return File(rishka_sc_1(RISHKA_SC_FS_NEXT, (i64) this->handle));
}

bool File::is_ok() {
    return rishka_sc_1(RISHKA_SC_FS_IS_OK, (i64) this->handle);
}

void File::flush() {
    rishka_sc_1(RISHKA_SC_FS_FLUSH, (i64) this->handle);
}

void File::close() {
    rishka_sc_1(RISHKA_SC_FS_CLOSE, (i64) this->handle);
}

bool File::bufsize(usize size) {
    return (bool) rishka_sc_2(RISHKA_SC_FS_BUFSIZE, (i64) this->handle, (i64) size);
}

u64 File::lastwrite() {
    return (u64) rishka_sc_1(RISHKA_SC_FS_LASTWRITE, (i64) this->handle);
}

bool File::seek_dir(u64 position) {
    return (bool) rishka_sc_2(RISHKA_SC_FS_SEEKDIR, (i64) this->handle, (i64) position);
}

string File::next_name() {
    return get_rt_string(rishka_sc_1(RISHKA_SC_FS_NEXT_NAME, (i64) this->handle));
}

void File::rewind() {
    rishka_sc_1(RISHKA_SC_FS_REWIND, (i64) this->handle);
}

bool FS::mkdir(const char* path) {
    return (bool) rishka_sc_1(RISHKA_SC_FS_MKDIR, (i64) path);
}

bool FS::rmdir(const char* path) {
    return (bool) rishka_sc_1(RISHKA_SC_FS_RMDIR, (i64) path);
}

bool FS::remove(const char* path) {
    return (bool) rishka_sc_1(RISHKA_SC_FS_DELETE, (i64) path);
}

bool FS::exists(const char* path) {
    return (bool) rishka_sc_1(RISHKA_SC_FS_EXISTS, (i64) path);
}