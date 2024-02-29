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