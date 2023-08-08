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

    while ((opt = getopt(argc, argv, ":m:h:e:")) != -1) {
        switch (opt) {
            case 'm':
                optsFound += 2;
                minSizeString = optarg;//This has an optarg
                break;
            case 'e':
                optsFound += 2;
                excludePaths.push_back(optarg);
                break;
            case 'h':
            case '?':
                result = PrintUsage;
                return;
            case ':':
                return;
            default:
                std::cerr << "Error, illegal argument " << (char)optopt << std::endl;
                return;
        }
    }

    if (argc < (optsFound + MinNumArgs))
    {
        std::cerr << "Error: arguments supplied incorrectly" << std::endl;
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

const std::vector<std::string>& ArgParser::getExcludePaths()
{
    return excludePaths;
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
