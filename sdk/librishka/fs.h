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

/**
 * @file fs.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Header file for file system operations in Rishka applications.
 *
 * This header file defines the File and FS classes, which provide functionalities
 * for interacting with files and directories in the file system on ESP32-WROVER microcontrollers.
 */

#ifndef LIBRISHKA_FS_H
#define LIBRISHKA_FS_H

#include <librishka/types.h>

/**
 * @class File
 * @brief Class for handling file operations in Rishka applications.
 *
 * The File class provides methods for performing various file operations,
 * such as opening, reading, writing, and closing files. It also includes
 * functionalities for file navigation and querying file attributes.
 */
class File final {
private:
    i32 handle;         /**< File handle for accessing file operations */

protected:
    File(i32 _handle);  /**< Protected constructor for creating a file object */

public:
    /**
     * @brief Open a file with the specified name and mode.
     *
     * This method opens a file with the given name and mode. The mode
     * parameter specifies the access mode for the file (e.g., "r" for reading,
     * "w" for writing).
     *
     * @param file The name of the file to open.
     * @param mode The mode in which to open the file (e.g., "r" for reading, "w" for writing).
     * @return A File object representing the opened file.
     */
    static File open(string file, string mode);

    /**
     * @brief Check if the object represents a file.
     *
     * This method checks whether the File object represents a file.
     *
     * @return True if the object represents a file, false otherwise.
     */
    bool is_file();

    /**
     * @brief Check if the object represents a directory.
     *
     * This method checks whether the File object represents a directory.
     *
     * @return True if the object represents a directory, false otherwise.
     */
    bool is_dir();

    /**
     * @brief Check if the object represents a directory.
     *
     * This method checks whether the File object represents a directory.
     *
     * @return True if the object represents a directory, false otherwise.
     */
    i32 available();

    /**
     * @brief Peek at the next byte in the file without consuming it.
     *
     * This method peeks at the next byte in the file without consuming it.
     *
     * @return The next byte in the file, or -1 if the end of the file is reached.
     */
    i32 peek();

    /**
     * @brief Get the timestamp of the last write operation on the file.
     *
     * This method returns the timestamp of the last write operation performed on the file.
     *
     * @return The timestamp of the last write operation.
     */
    u64 lastwrite();

    /**
     * @brief Set the buffer size for the file.
     *
     * This method sets the buffer size for the file. It determines the
     * amount of data buffered in memory before being written to the file.
     *
     * @param size The buffer size to set, in bytes.
     * @return True if the buffer size was successfully set, false otherwise.
     */
    bool bufsize(usize size);

    /**
     * @brief Set the current position within the file.
     *
     * This method sets the current position within the file to the specified position.
     *
     * @param pos The position to set.
     * @return True if the position was successfully set, false otherwise.
     */
    bool seek(u32 pos);

    /**
     * @brief Set the current position within the directory.
     *
     * This method sets the current position within the directory to the specified position.
     *
     * @param position The position to set.
     * @return True if the position was successfully set, false otherwise.
     */
    bool seek_dir(u64 position);

    /**
     * @brief Get the size of the file.
     *
     * This method returns the size of the file.
     *
     * @return The size of the file, in bytes.
     */
    usize size();

    /**
     * @brief Get the current position within the file.
     *
     * This method returns the current position within the file.
     *
     * @return The current position within the file.
     */
    usize position();

    /**
     * @brief Read a byte from the file.
     *
     * This method reads a byte from the file and advances the file pointer.
     *
     * @return The byte read from the file, or -1 if the end of the file is reached.
     */
    i32 read();

    /**
     * @brief Write a byte to the file.
     *
     * This method writes a byte to the file at the current position.
     *
     * @param data The byte to write to the file.
     */
    void write(u8 data);

    /**
     * @brief Write a string to the file.
     *
     * This method writes a string to the file at the current position.
     *
     * @param data The string to write to the file.
     */
    void write(string data);

    /**
     * @brief Get the path of the file.
     *
     * This method returns the path of the file.
     *
     * @return The path of the file.
     */
    string path();

    /**
     * @brief Get the name of the file.
     *
     * This method returns the name of the file.
     *
     * @return The name of the file.
     */
    string name();

    /**
     * @brief Get the next file in the directory.
     *
     * This method returns the next file in the directory and advances
     * the directory pointer. The mode parameter specifies the mode in
     * which to open the next file (e.g., "r" for reading, "w" for writing).
     *
     * @param mode The mode in which to open the next file (e.g., "r" for reading, "w" for writing).
     * @return A File object representing the next file in the directory.
     */
    File next(string mode);

    /**
     * @brief Get the name of the next file in the directory.
     *
     * This method returns the name of the next file in the directory
     * without advancing the directory pointer.
     *
     * @return The name of the next file in the directory.
     */
    string next_name();

    /**
     * @brief Flush the file buffer.
     *
     * This method flushes the file buffer, writing any buffered data to the file.
     */
    void flush();

    /**
     * @brief Close the file.
     *
     * This method closes the file, releasing any associated resources.
     */
    void close();

    /**
     * @brief Rewind the file pointer to the beginning of the file.
     *
     * This method rewinds the file pointer to the beginning of the file.
     */
    void rewind();
};

/**
 * @class FS
 * @brief Class for file system operations in Rishka applications.
 *
 * The FS class provides static methods for performing file system operations,
 * such as creating, deleting, and checking the existence of directories and files.
 */
class FS final {
public:
    /**
     * @brief Create a directory with the specified path.
     *
     * This method creates a directory with the given path.
     *
     * @param path The path of the directory to create.
     * @return True if the directory was successfully created, false otherwise.
     */
    static bool mkdir(const char* path);

    /**
     * @brief Remove a directory with the specified path.
     *
     * This method removes a directory with the given path.
     *
     * @param path The path of the directory to remove.
     * @return True if the directory was successfully removed, false otherwise.
     */
    static bool rmdir(const char* path);

    /**
     * @brief Remove a file with the specified path.
     *
     * This method removes a file with the given path.
     *
     * @param path The path of the file to remove.
     * @return True if the file was successfully removed, false otherwise.
     */
    static bool remove(const char* path);

    /**
     * @brief Check if a file or directory exists at the specified path.
     *
     * This method checks if a file or directory exists at the given path.
     *
     * @param path The path to check.
     * @return True if a file or directory exists at the specified path, false otherwise.
     */
    static bool exists(const char* path);
};

#endif /* LIBRISHKA_FS_H */