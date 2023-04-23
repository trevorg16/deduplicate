#define _FILE_OFFSET_BITS 64

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>

#include "file_list.h"

const long MAXIMUM_PATH_LENGTH = 256;

FileList::FileList(const char* p, const off_t min) : minSize(min), path(p)
{
}

void FileList::recurseFiles()
{
    const long maximumPathLength = pathconf(path, _PC_PATH_MAX);
    localFileTreeWalk(path, maximumPathLength);
}

PathList& FileList::getFileList()
{
    return fileList;
}

void FileList::localFileTreeWalk(const char *path, long maximumPathLength)
{
    DIR *dir;
    struct dirent* entry;

    char next_path[MAXIMUM_PATH_LENGTH + 1] = {};

    if(!(dir = opendir(path))){
        return;
    }

    do{
        entry = readdir(dir);

        if(entry){
            if (entry->d_type == DT_DIR && ( strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)){
                //Skip this one
            }
            else if (entry->d_type == DT_DIR){
                concatPath(path, entry->d_name, next_path, std::min(maximumPathLength, MAXIMUM_PATH_LENGTH));

                const long newPathLength = maximumPathLength - strlen(entry->d_name);
                localFileTreeWalk(next_path, newPathLength);
            }
            else
            {
                concatPath(path, entry->d_name, next_path, std::min(maximumPathLength, MAXIMUM_PATH_LENGTH));
                addFileToPathList(next_path, entry->d_type);
            }
        }
    } while (entry);

    closedir(dir);
}

void FileList::addFileToPathList(const char* filePath, unsigned char dtype)
{
    struct stat sb = {};

    if (dtype != DT_REG)
    {
        return;
    }

    if (stat(filePath, &sb) != 0)
    {
        // failed to process file
        return;
    }

    const off_t fSize = sb.st_size;

    if (fSize > minSize)
    {
        const FileInfo info = {std::string(filePath), fSize};

        fileList.push_back(info);
    }
}

bool FileList::concatPath(const char* basePath, const char* filePath, char* concatenatedPath, long maxPathLength)
{
    size_t written = snprintf(concatenatedPath, maxPathLength, "%s/%s", basePath, filePath);
    return static_cast<long>(written) < maxPathLength;
}