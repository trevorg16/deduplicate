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

#include <iostream>

#include "arg_parser.h"
#include "duplicate_finder.h"
#include "ui.h"


void usage(char* progName);

int main(int argc, char* argv[])
{
    ArgParser parser;
    parser.parse(argc, argv);

    if (parser.getResult() == RunDedup)
    {
        UI ui;
        DuplicateFinder duplicateFinder(parser.getPath(), parser.getMinSize(), 10 * KB_PER_MB * BYTES_PER_KB, 10 * KB_PER_MB * BYTES_PER_KB, true, &ui);

        ui.run();

        duplicateFinder.join();
    }
    else if (parser.getResult() == PrintUsage)
    {
        usage(argv[0]);
    }
    else
    {
        usage(argv[0]);
        return 1;
    }

    return 0;
}

void usage(char* progname)
{
    std::cerr << progname << " - find duplicated files in the specified directory by scanning the first and last 10MB of the file" << std::endl;
    std::cerr << "Usage:" << std::endl;
    std::cerr << progname << " [Options] Path" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Options: " << std::endl;
    std::cerr << "\t" << "-m SIZE[SIZE_SUFFIX]\tThe minimum file size to consider for deduplication" << std::endl;
    std::cerr << "\t" << "SIZE_SUFFIX\tValid suffixes: [KB, MB, GB, TB], default bytes" << std::endl;
}