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

#include "file_info_key.h"

#define _FILE_OFFSET_BITS 64

#include <sys/types.h>
#include <openssl/md5.h>
#include <stdint.h>
#include <cstring>


bool FileInfoKey::lessThan(const FileInfoKey& lhs, const FileInfoKey& rhs)
{
    if (lhs.file_size != rhs.file_size)
    {
        return lhs.file_size < rhs.file_size;
    }

    const int res = memcmp(lhs.md5.data(), rhs.md5.data(), MD5_DIGEST_LENGTH);

    return res < 0;
}

bool FileInfoKey::greaterThan(const FileInfoKey& lhs, const FileInfoKey& rhs)
{
    if (lhs.file_size != rhs.file_size)
    {
        return lhs.file_size > rhs.file_size;
    }

    const int res = memcmp(lhs.md5.data(), rhs.md5.data(), MD5_DIGEST_LENGTH);

    return res > 0; 
}