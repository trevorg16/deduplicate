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

#ifndef DUPLICATE_FINDER_H
#define DUPLICATE_FINDER_H

#define _FILE_OFFSET_BITS 64

#include <sys/types.h>
#include <unordered_map>
#include <map>
#include <thread>
#include "file_list.h"

typedef std::vector<std::string> FilenameList;
typedef std::map<FileInfoKey, FilenameList, FileInfoKeyLessThan> PathMap;

class UI;


// Build an intermediary map of all of the files, then go through that list
// and calculate the hash
class DuplicateFinder
{
    public:
        DuplicateFinder(const char* p, const off_t min, const std::vector<std::string>& exclude, const off_t maxHeader, const off_t maxTail, bool skipMiddle, UI* ui);
        void join();
        PathMap* getFileMap();

    private:
        void getFileHash(const char* filename, const off_t fileSize);
        void addToVectorHashMap(FileInfoKey fileInfo, const char* path);

        void keepOnlyDuplicates();

        static void findDuplicates(DuplicateFinder* finder);

        FileList fileList;

        PathMap fileMap;

        off_t maxHeader;
        off_t maxTail;
        bool skipMiddle;

        UI* uiMessagePass;

        std::thread worker;
};

#endif /* DUPLICATE_FINDER_H */
