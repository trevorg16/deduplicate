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
        FileList(const char* p, const off_t min, const std::vector<std::string>& exclude);

        void recurseFiles();
        PathList& getFileList();
    private:
        void localFileTreeWalk(const char *path, long maximumPathLength);
        void addFileToPathList(const char* filePath, unsigned char dtype);
        static bool concatPath(const char* basePath, const char* filePath, char* concatenatedPath, long maxPathLength);
        bool isInExcludeList(const std::string& path);

        PathList fileList;
        off_t minSize;
        const char* path;
        const std::vector<std::string>& excludePaths;
};

#endif /* DUPLICATE_MAP_H */
