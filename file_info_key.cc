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