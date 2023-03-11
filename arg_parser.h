#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#define _FILE_OFFSET_BITS 64

#include <sys/types.h>

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