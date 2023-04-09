#ifndef DUPLICATE_FINDER_H
#define DUPLICATE_FINDER_H

#define _FILE_OFFSET_BITS 64

#include <sys/types.h>
#include <unordered_map>
#include <map>
#include <thread>
#include "file_list.h"
// #include "ui.h"

typedef std::vector<std::string> FilenameList;
// typedef std::unordered_map<FileInfoKey, FilenameList, FileInfoKeyHash, FileInfoKeyEqual> PathUnorderedMap;
// typedef std::unordered_map<FileInfoKey, FilenameList, FileInfoKeyHash, FileInfoKeyEqual> PathUnorderedMap;
typedef std::map<FileInfoKey, FilenameList, FileInfoKeyLessThan> PathMap;
// typedef std::pair<FileInfoKey, PathList> FileInfoElement_t;
// typedef std::vector<FileInfoElement_t> PathOrderedList_t;

class UI;


// Build an intermediary map of all of the files, then go through that list
// and calculate the hash
class DuplicateFinder
{
    public:
        DuplicateFinder(const char* p, const off_t min, const off_t maxHeader, const off_t maxTail, bool skipMiddle, UI* ui);
        void join();
        PathMap* getFileMap();

    private:
        void getFileHash(const char* filename, const off_t fileSize);
        void addToVectorHashMap(FileInfoKey fileInfo, const char* path);

        void keepOnlyDuplicates();
        
        static void findDuplicates(DuplicateFinder* finder);

        FileList fileList;
        // PathUnorderedMap hashedFileMap;
        PathMap fileMap;
        
        off_t maxHeader;
        off_t maxTail;
        bool skipMiddle;

        UI* uiMessagePass;

        std::thread worker;
};

#endif /* DUPLICATE_FINDER_H */