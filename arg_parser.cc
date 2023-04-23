#define _FILE_OFFSET_BITS 64

#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <iostream>

#include "arg_parser.h"

static const int MinNumArgs = 2;
static const off_t DefaultMinSize = 0;

ArgParser::ArgParser() : 
    result(PrintUsageError),
    minSize(0),
    path(NULL)
{
}

void ArgParser::parse(int argc, char* argv[])
{
    // Default to print usage and error
    result = PrintUsageError;

    if (argc < MinNumArgs)
    {
        return;
    }

    int opt;
    char *minSizeString = NULL;

    int optsFound = 0;

    while ((opt = getopt(argc, argv, ":m:h")) != -1) {
        switch (opt) {
            case 'm':
                optsFound += 2;
                minSizeString = optarg;//This has an optarg
                break;
            case 'h':
            case '?':
                result = PrintUsage;
                return;
            case ':':
                std::cerr << "Error, missing argument for " << (char)optopt << std::endl;
                return;
            default:
                std::cerr << "Error, illegal argument " << (char)optopt << std::endl;
                return;
        }
    }

    if (argc < (optsFound + MinNumArgs))
    {
        std::cerr << "Error -m argument supplied without SIZE or PATH" << std::endl;
        result = PrintUsageError;
        return;
    }
    else
    {
        path = argv[optsFound + MinNumArgs - 1];
    }

    result = RunDedup;

    setMinSize(minSizeString, DefaultMinSize);
}

ArgParserResult ArgParser::getResult()
{
    return result;
}

off_t ArgParser::getMinSize()
{
    return minSize;
}

const char* ArgParser::getPath()
{
    return path;
}

void ArgParser::setMinSize(char* strSize, off_t defaultBytes)
{
    off_t numBytes = defaultBytes;

    if(strSize != NULL){
        long long base_val = atoll(strSize);

        char* suffix = strSize;
        while(*suffix == ' ' || (*suffix >= '0' && *suffix <= '9')) suffix++;

        if(strncmp("kB", suffix, 3) == 0){
            numBytes = base_val * BYTES_PER_KB;
        }
        else if(strncmp("MB", suffix, 3) == 0){
            numBytes = base_val * KB_PER_MB * BYTES_PER_KB;
        }
        else if(strncmp("GB", suffix, 3) == 0){
            numBytes = base_val * MB_PER_GB * KB_PER_MB * BYTES_PER_KB;
        }
        else if(strncmp("TB", suffix, 3) == 0){
            numBytes = base_val * GB_PER_TB * MB_PER_GB * KB_PER_MB * BYTES_PER_KB;
        }
        else if(*suffix == '\0'){//If we've already reached the NULL terminnation then assume it is bytes
            numBytes = base_val;
        }
        else{
            numBytes = defaultBytes;
        }
    }

    minSize = numBytes;
}
