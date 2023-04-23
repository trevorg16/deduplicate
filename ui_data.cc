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

#include "ui_data.h"
#include <cstring>
#include <iterator>
#include <iostream>

#define HEADER_SIZE_Y (1)
#define HEADER_SIZE_X (0)

#define WINDOW_BORDER_SIZE (1)

#define THREASHOLD_SIZE (890)
#define SIZE_MULTIPLE_CONSTANT (1024)

UIData::UIData() :
    window(NULL),
    pathMap(NULL),
    pathMapMutex(),
    firstRun(true),
    quit(false)
{
}

UIData::~UIData()
{
    destroy();
}

void UIData::create(int lines, int cols)
{
    destroy();

    nlines = lines;
    ncols = cols;

    x_scroll = 0;

    running = true;

    window = newwin(lines - HEADER_SIZE_Y, cols - HEADER_SIZE_X, HEADER_SIZE_Y, HEADER_SIZE_X);
}

void UIData::destroy()
{
    if (window)
    {
        delwin(window);
        window = NULL;
    }
}

void UIData::shiftUp(UILocation& location)
{
    std::reverse_iterator<PathMap::iterator>& entry = location.mainEntry;

    if (location.subEntry > 0)
    {
        location.subEntry--;
    }
    else if (entry != pathMap->rbegin())
    {
        entry--;
        location.subEntry = entry->second.size() - 1;
    }
}

void UIData::scrollUp()
{
    shiftUp(select_line);

    while (select_line < top_line)
    {
        shiftUp(top_line);
    }
}

void UIData::shiftDown(UILocation& location)
{
    std::reverse_iterator<PathMap::iterator>& entry = location.mainEntry;
    
    if (entry->second.size() > 0 &&
        location.subEntry < (entry->second.size() - 1))
    {
        location.subEntry++;
    }
    else
    {
        UILocation newLocation = location;
        newLocation.mainEntry++;

        if (newLocation.mainEntry != pathMap->rend())
        {
            newLocation.subEntry = 0;
            location = newLocation;
        }
    }
}

void UIData::scrollDown()
{
    shiftDown(select_line);

    if (select_line > bottom_line)
    {
        shiftDown(top_line);
    }
}

void UIData::drawScreen(PathMap* map)
{
    werase(window);
    box(window, 0, 0);

    int windowHeight = nlines - HEADER_SIZE_Y - 2;//Include space for the border

    int currentLine = 1;
    const unsigned int size_tab_sz = 16;

    PathMap::reverse_iterator currentMainEntryIter = top_line.mainEntry;
    FilenameList::size_type current_sub_entry = top_line.subEntry;

    while(currentLine <= windowHeight && currentMainEntryIter != map->rend())
    {
        const auto& currentMainEntry = *currentMainEntryIter;

        char sizeValueToPrint[size_tab_sz + 1];
        byteSizeToStr(currentMainEntry.first.file_size, sizeValueToPrint, size_tab_sz + 1);
        mvwprintw(window, currentLine, WINDOW_BORDER_SIZE,"%s", sizeValueToPrint);
        //currentLine++;
        while(currentLine <= windowHeight && currentMainEntry.second.size() > current_sub_entry){
            const char* filename_all = currentMainEntry.second[current_sub_entry].c_str();
            char filename_view[ncols - size_tab_sz - 1]; // variable array
            int safe_offset = x_scroll;
            if(safe_offset < 0){
                safe_offset = 0;
            }

            else if(static_cast<long unsigned int>(safe_offset) > currentMainEntry.second[current_sub_entry].length()){
                safe_offset = 0;
                filename_all = "";
            }

            strncpy(filename_view, filename_all + safe_offset, ncols - size_tab_sz - 2);//Leave space for the box
            filename_view[ncols - size_tab_sz - 2] = '\0';
            
            if(select_line.mainEntry == currentMainEntryIter && select_line.subEntry == current_sub_entry)
            {
                wattron(window, A_REVERSE);
            }

            mvwaddstr(window, currentLine, WINDOW_BORDER_SIZE + size_tab_sz, filename_view);
            
            if(select_line.mainEntry == currentMainEntryIter && select_line.subEntry == current_sub_entry)
            {
                wattroff(window, A_REVERSE);
            }

            bottom_line.mainEntry = currentMainEntryIter;
            bottom_line.subEntry = current_sub_entry;

            currentLine++;
            current_sub_entry++;
        }
        currentMainEntryIter++;
        current_sub_entry = 0;
    }

    refresh();
    wrefresh(window);
}

void UIData::draw()
{
    PathMap* map = NULL;

    pathMapMutex.lock();
    map = pathMap;
    pathMapMutex.unlock();

    if (!map)
    {
        return;
    }

    if (firstRun)
    {
        firstRun = false;
        top_line.mainEntry = map->rbegin();
        top_line.subEntry = 0;

        select_line = top_line;
    }

    while(running)
    {
        drawScreen(map);

        int c = -1;

        while (c == -1)
        {
            c = getch();
        }

        switch (c)
        {
            case 'q':
            case 'Q':
            {
                quit = true;
                running = false;
                break;
            }
            case KEY_RESIZE:
            {
                running = false;
                break;
            }
            case KEY_UP:
            case 'k':
            {
                scrollUp();
                break;
            }
            case KEY_DOWN:
            case KEY_ENTER:
            case 'j':
            {
                scrollDown();
                break;
            }
            case KEY_RIGHT:
            case 'l':
            {
                x_scroll++;
                break;
            }
            case KEY_LEFT:
            case 'h':
            {
                if (x_scroll > 0)
                {
                    x_scroll--;
                }
                break;
            }
            case '0':
            case '^':
            case KEY_HOME:
            {
                x_scroll = 0;
                break;
            }
            case 'g':
            {
                select_line.mainEntry = pathMap->rbegin();
                select_line.subEntry = 0;

                top_line = select_line;
                break;
            }
            case 'G':
            {
                select_line.mainEntry = pathMap->rend();
                select_line.mainEntry--;

                select_line.subEntry = select_line.mainEntry->second.size() - 1;

                if (select_line > bottom_line)
                {
                    top_line = select_line;
                }

                break;
            }
            case KEY_PPAGE://Page up
            {
                if (select_line > top_line)
                {
                    select_line = top_line;
                }
                else
                {
                    for (int i = 0; i < (nlines - HEADER_SIZE_Y); i++)
                    {
                        scrollUp();
                    }
                }
                break;
            }
            case KEY_NPAGE://Page down
            {
                select_line = bottom_line;
                shiftDown(select_line);
                if (select_line > bottom_line)
                {
                    top_line = select_line;
                }

                break;
            }
        }
    }
}

void UIData::updateData(PathMap* map)
{
    pathMapMutex.lock();

    pathMap = map;

    pathMapMutex.unlock();
}

bool UIData::didQuit()
{
    return quit;
}

void UIData::byteSizeToStr(off_t byteSize, char* dest, size_t maxStringLen){
    if(dest == NULL){
        return;
    }

    const char* postFixList[] = {"b", "kB", "MB", "GB", "TB", NULL};
    const char** postFix = postFixList;
    float divisor = 1;

    while(postFix != NULL){
        if(((float)byteSize / divisor) <= THREASHOLD_SIZE){
            break;
        }
        divisor *= SIZE_MULTIPLE_CONSTANT;
        postFix++;
        if(*postFixList == NULL){
            const char* infSizeString = "inf";
            strncpy(dest, infSizeString, maxStringLen);
            dest[maxStringLen - 1] = '\0';//This guarantees that the string is terminated
            return;
        }
    }

    snprintf(dest, maxStringLen, "%.2f %s", ((float)byteSize / divisor), *postFix);
}
