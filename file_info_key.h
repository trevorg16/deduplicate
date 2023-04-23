/*
 * Copyright 2023 Trevor Gale
 *
 * This file is part of deduplicate
 *
 * deduplicate is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * deduplicate is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with MacWords. If not,
 * see <https://www.gnu.org/licenses/>. 
 */

#ifndef FILE_INFO_KEY_H
#define FILE_INFO_KEY_H

#define _FILE_OFFSET_BITS 64

#include <sys/types.h>
#include <openssl/md5.h>

#include <array>

typedef std::array<uint8_t, MD5_DIGEST_LENGTH> Md5_Dat;

// Key elements to be used in an unordered_map
class FileInfoKey
{    
    public:
        static bool lessThan(const FileInfoKey& lhs, const FileInfoKey& rhs);
        static bool greaterThan(const FileInfoKey& lhs, const FileInfoKey& rhs);

        Md5_Dat md5;
        off_t file_size;
};

struct FileInfoKeyLessThan {
    bool operator()(const FileInfoKey& lhs, const FileInfoKey& rhs) const
    {
        return FileInfoKey::lessThan(lhs, rhs);
    }
};

#endif /* FILE_INFO_KEY_H */