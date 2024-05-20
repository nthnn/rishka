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

/**
 * @file rishka_util.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Rishka Utility Functions
 *
 * This file contains utility functions for converting value
 * between double precision floating-point and long integer values.
 */

#ifndef RISHKA_UTIL_H
#define RISHKA_UTIL_H

#include <rishka_types.h>

/**
 * @brief Converts a double value to a long integer.
 *
 * This function converts a double precision floating-point value `d` to a long integer and returns it.
 *
 * @param d The double precision floating-point value to be converted.
 * @return The converted long integer value.
 */
inline int64_t rishka_double_to_long(double d) {
    union {
        double input;
        int64_t output;
    } data;

    data.input = d;
    return data.output;
}

/**
 * @brief Converts a long integer to a double value.
 *
 * This function converts a long integer `l` to a double precision floating-point value and returns it.
 *
 * @param l The long integer value to be converted.
 * @return The converted double precision floating-point value.
 */
inline double rishka_long_to_double(int64_t l) {
    union {
        double output;
        int64_t input;
    } data;

    data.input = l;
    return data.output;
}

/**
 * @brief Sanitizes a file path by resolving it relative to the current working directory.
 *
 * This function sanitizes a file path by resolving it relative to the current working directory.
 * If the provided path is already absolute, it is returned as is. Otherwise, it is resolved
 * relative to the current working directory.
 *
 * @param currentWorkingDirectory The current working directory.
 * @param path The file path to sanitize.
 * @return A String containing the sanitized file path.
 */
inline String rishka_sanitize_path(String currentWorkingDirectory, char* path) {
    if(strlen(path) == 1 && path[0] == '~')
        return "/";

    if(strcmp(currentWorkingDirectory.c_str(), path) == 0)
        return currentWorkingDirectory;

    if(strlen(path) >= 2 && path[0] == '~' && path[1] == '/')
        return String(path).substring(1);

    String segment, sanitizedPath = "",
        fullPath = (currentWorkingDirectory == "/" ?
            "" : currentWorkingDirectory) +
                "/" + String(path);

    if(strcmp(path, "..") == 0)
        fullPath += "/";

    int index;
    while((index = fullPath.indexOf('/')) != -1) {
        segment = fullPath.substring(0, index);
        fullPath = fullPath.substring(index + 1);

        if(segment == ".");
        else if(segment == "..") {
            int lastSlashIndex = sanitizedPath.lastIndexOf('/');

            if(lastSlashIndex != -1)
                sanitizedPath = sanitizedPath.substring(0, lastSlashIndex);
        }
        else sanitizedPath += "/" + segment;
    }

    if(fullPath != "." && fullPath != "..")
        sanitizedPath += "/" + fullPath;

    if(sanitizedPath.startsWith("//"))
        sanitizedPath = sanitizedPath.substring(1);    

    if(sanitizedPath.endsWith("/") && strlen(sanitizedPath.c_str()) > 1)
        sanitizedPath = sanitizedPath.substring(0, sanitizedPath.length() - 1);

    sanitizedPath.replace("//", "/");
    return sanitizedPath;
}

inline void rishka_split_cmd(const String& input, char** tokens, int maxTokens, int &count) {
    int tokenCount = 0, tokenStart = 0;
    bool inQuotes = false;

    for(int i = 0; i < input.length(); i++) {
        if(input[i] == '"')
            inQuotes = !inQuotes;
        else if(input[i] == ' ' && !inQuotes) {
            if(tokenCount < maxTokens) {
                input.substring(tokenStart, i)
                    .toCharArray(tokens[tokenCount], i - tokenStart + 1);

                tokens[tokenCount++][i - tokenStart] = '\0';
                tokenStart = i + 1;
                count++;
            }
            else break;
        }
    }

    if(tokenCount < maxTokens && tokenStart < input.length()) {
        input.substring(tokenStart).toCharArray(
            tokens[tokenCount],
            input.length() - tokenStart + 1
        );

        tokens[tokenCount++][input.length() - tokenStart] = '\0';
        count++;
    }
}

#endif /* RISHKA_UTIL_H */