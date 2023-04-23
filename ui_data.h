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

#ifndef UI_DATA_HH
#define UI_DATA_HH

#define _FILE_OFFSET_BITS 64

#include <mutex>
#include <atomic>
#include <curses.h>

#include "duplicate_finder.h"

struct UILocation
{
    PathMap::reverse_iterator mainEntry;
    FilenameList::size_type subEntry;

    bool operator> (const UILocation& b) const {
        const UILocation& a = *this;

        if (a.mainEntry != b.mainEntry){
            return FileInfoKey::lessThan(a.mainEntry->first, b.mainEntry->first);
        }
        else{
            return a.subEntry > b.subEntry;
        }
    }

    bool operator< (const UILocation &b){
        const UILocation& a = *this;

        if (this->mainEntry != b.mainEntry){
            return FileInfoKey::greaterThan(a.mainEntry->first, b.mainEntry->first);
        }
        else{
            return this->subEntry < b.subEntry;
        }
    }
};

class UIData
{
    public:

    UIData();

    ~UIData();

    void create(int lines, int cols);

    void destroy();

    void draw();

    void updateData(PathMap* map);

    bool didQuit();

    private:

    void scrollUp();

    void shiftDown(UILocation& location);

    void shiftUp(UILocation& location);

    void scrollDown();

    void drawScreen(PathMap* map);

    static void byteSizeToStr(off_t byteSize, char* dest, size_t maxStringLen);

    WINDOW* window;

    PathMap* pathMap;

    std::mutex pathMapMutex;

    int x_scroll;

    UILocation top_line;
    UILocation bottom_line;
    UILocation select_line;

    bool firstRun;

    bool running;
    bool quit;

    int nlines;
    int ncols;
};

#endif /* UI_DATA_HH */