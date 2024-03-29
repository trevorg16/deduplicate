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

#define _FILE_OFFSET_BITS 64

#include <sys/types.h>
#include <openssl/md5.h>

#include <unordered_map>
#include <iostream>
#include <algorithm>

#include "duplicate_finder.h"
#include "ui.h"

#define MD5_BUF_LEN (8192)

DuplicateFinder::DuplicateFinder(const char* p,  const off_t min, const std::vector<std::string>& exclude, const off_t maxHeader, const off_t maxTail, bool skipMiddle, UI* ui) :
    fileList(p, min, exclude),
    maxHeader(maxHeader),
    maxTail(maxTail),
    skipMiddle(skipMiddle),
    uiMessagePass(ui)
{
    worker = std::thread(findDuplicates, this);
}

void DuplicateFinder::findDuplicates(DuplicateFinder* finder)
{
    finder->fileList.recurseFiles();
    const PathList& pathList = finder->fileList.getFileList();

    for (auto& path : pathList)
    {
        finder->getFileHash(path.first.c_str(), path.second);
    }

    finder->keepOnlyDuplicates();

    finder->uiMessagePass->finderComplete(finder);
}

void DuplicateFinder::getFileHash(const char* filename, const off_t fileSize){
    if (fileSize <= 0)
    {
        return;
    }

    FILE* fp = fopen(filename, "r");

    if(fp == NULL)
    {
        return;
    }

    uiMessagePass->updatePath(filename);

    bool hadError = false;

    MD5_CTX c = {};
    char buf[MD5_BUF_LEN];
    unsigned char out[MD5_DIGEST_LENGTH];

    MD5_Init(&c);

    if( skipMiddle && (fileSize > (maxHeader + maxTail)))
    {
        // Need to skip the middle
        off_t remainingBytes = maxHeader;
        size_t readSz = 0;
        
        while(remainingBytes > 0 && (readSz = fread(buf, sizeof(char), std::min(static_cast<off_t>(MD5_BUF_LEN), remainingBytes), fp)) > 0){
            remainingBytes -= readSz;
            MD5_Update(&c, buf, readSz);
        }

        if(readSz == 0){
            std::cerr << "Error reading file: " << filename << std::endl;
            hadError = true;
        }

        remainingBytes = maxTail;

        //Go to fsize - SIZE_TO_READ_FROM_TAIL_IF_SKIP
        fseek(fp, fileSize - maxTail, SEEK_SET);

        while(remainingBytes > 0 && (readSz = fread(buf, sizeof(char), std::min(static_cast<off_t>(MD5_BUF_LEN), remainingBytes), fp)) > 0){
            remainingBytes -= readSz;
            MD5_Update(&c, buf, readSz);
        }

        if(readSz == 0 /*|| feof(fp) == 0*/){
            // std::cerr << "Did not hit eof on " << filename << std::endl;
            hadError = true;
        }
    }
    else{
        size_t readSz;

        while((readSz = fread(buf, sizeof(char), MD5_BUF_LEN, fp)) > 0){
            //cout << "Read of: " << pathname << endl;
            MD5_Update(&c, buf, readSz);
        }

        if(feof(fp) == 0){
            // std::cerr << "Could not read: " << filename << std::endl;
            hadError = true;
        }
    }
    
    MD5_Final(out, &c);

    fclose(fp);

    if (!hadError)
    {
        FileInfoKey fileInfo;
        std::copy(std::begin(out), std::end(out), std::begin(fileInfo.md5));
        fileInfo.file_size = fileSize;
        addToVectorHashMap(fileInfo, filename);
    }
}

void DuplicateFinder::addToVectorHashMap(FileInfoKey fileInfo, const char* path)
{
    if(fileMap.count(fileInfo) > 0)
    {
        fileMap[fileInfo].push_back(path);
    }
    else
    {
        FilenameList newPathList;
        newPathList.push_back(path);

        fileMap[fileInfo] = newPathList;
    }
}

void DuplicateFinder::keepOnlyDuplicates()
{
    auto it = fileMap.begin();

    while (it != fileMap.end())
    {
        if (it->second.size() < 2)
        {
            fileMap.erase(it++);
        }
        else
        {
            ++it;
        }
    }
}

void DuplicateFinder::join()
{
    worker.join();
}

PathMap* DuplicateFinder::getFileMap()
{
    return &fileMap;
}
