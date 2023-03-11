#ifndef DUPLICATE_MAP_H
#define DUPLICATE_MAP_H

#define _FILE_OFFSET_BITS 64

#include <sys/types.h>

#include <vector>
#include <string>

#include "file_info_key.h"

typedef std::pair<std::string, const off_t> FileInfo;
typedef std::vector<FileInfo> PathList;


// Contains all logic related to building a map of duplicate file in the file system
class FileList
{
    public:
        FileList(const char* p, const off_t min);

        void recurseFiles();
        PathList& getFileList();
    private:
        void localFileTreeWalk(const char *path, long maximumPathLength);
        void addFileToPathList(const char* filePath, unsigned char dtype);
        static bool concatPath(const char* basePath, const char* filePath, char* concatenatedPath, long maxPathLength);

        // PathUnorderedMap fileHashMap; // Move this to something specific to the map, then keep the list local here
        PathList fileList;
        off_t minSize;
        const char* path;
};

#endif /* DUPLICATE_MAP_H */