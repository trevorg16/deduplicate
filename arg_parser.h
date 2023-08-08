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

#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#define _FILE_OFFSET_BITS 64

#include <sys/types.h>
#include <vector>
#include <string>

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
        const std::vector<std::string>& getExcludePaths();

    private:
        void setMinSize(char* strSize, off_t defaultBytes);

        ArgParserResult result;
        off_t minSize;
        const char* path;
        std::vector<std::string> excludePaths;
};

#endif /* ARG_PARSER_H */
