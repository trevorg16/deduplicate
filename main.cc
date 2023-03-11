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
        DuplicateFinder duplicateFinder(parser.getPath(), parser.getMinSize(), 100, 100, true, &ui);

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
    std::cerr << "Usage:" << std::endl;
    std::cerr << progname << " [Options] Path" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Options: " << std::endl;
    std::cerr << "\t" << "-m SIZE[SIZE_SUFFIX]\tThe minimum file size to consider for deduplication" << std::endl;
    std::cerr << "\t" << "SIZE_SUFFIX\tValid suffixes: [KB, MB, GB, TB], default bytes" << std::endl;
}