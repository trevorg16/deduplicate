#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#define _FILE_OFFSET_BITS 64

#include <sys/types.h>

static const int BYTES_PER_KB = 1024;
static const int KB_PER_MB    = 1024;
static const int MB_PER_GB    = 1024;
static const int GB_PER_TB    = 1024;

enum ArgParserResult
{
    PrintUsage,
    PrintUsageError,
    RunDedup
};

class ArgParser
{
    public:
        ArgParser();
        void parse(int argc, char* argv[]);
        ArgParserResult getResult();
        off_t getMinSize();
        const char* getPath();

    private:
        void setMinSize(char* strSize, off_t defaultBytes);

        ArgParserResult result;
        off_t minSize;
        const char* path;
};

#endif /* ARG_PARSER_H */