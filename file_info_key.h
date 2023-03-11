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