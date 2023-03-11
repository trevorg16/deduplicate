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

FileList::FileList(const char* p, const off_t min) : minSize(min), path(p)
{
}

void FileList::recurseFiles()
{
    // std::cout << "Running the recuseFiles function on " << path << std::endl;
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
                char subfolder_path[maximumPathLength]; // TODO: Fix this VLA
                concatPath(path, entry->d_name, subfolder_path, maximumPathLength);

                const long newPathLength = maximumPathLength - strlen(entry->d_name);
                localFileTreeWalk(subfolder_path, newPathLength);
            }
            else
            {
                // int maximumPathLength = pathconf(path, _PC_PATH_MAX);
                char file_path[maximumPathLength];
                concatPath(path, entry->d_name, file_path, maximumPathLength);
                addFileToPathList(file_path, entry->d_type);
                // struct stat sb = {};
                // if(stat(file_path, &sb) == 0){
                //     get_file_info(file_path, entry.d_type, path_size_list);
                // }
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
        std::cerr << "failed to process file: " << filePath << " errno: " << errno << std::endl;
        return;
    }

    const off_t fSize = sb.st_size;

    if (fSize > minSize)
    {
        const FileInfo info = {std::string(filePath), fSize};

        fileList.push_back(info);
        // file_count++;

        // FileInfoKey file_info;

        // unsigned char out[MD5_DIGEST_LENGTH];
        // memset(&out, 0x00, MD5_DIGEST_LENGTH);
        // std::copy(std::begin(out), std::end(out), std::begin(file_info.md5));
        // file_info.file_size = fSize;

        // addToVectorHashMap(hash_map, file_info, pathname);

    }
}

bool FileList::concatPath(const char* basePath, const char* filePath, char* concatenatedPath, long maxPathLength)
{
    size_t written = snprintf(concatenatedPath, maxPathLength, "%s/%s", basePath, filePath);
    return static_cast<long>(written) < maxPathLength;
}