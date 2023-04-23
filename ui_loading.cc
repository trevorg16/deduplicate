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

#include <algorithm>
#include <cstring>
#include <curses.h>

#include "ui_loading.h"

#define LOADING_LINES (8)
#define LOADING_COLUMNS (64)
#define LOADING_STRING "LOADING"

UILoading::UILoading() : 
    window(NULL),
    pathWaiting(false),
    pathMutex()
{
    path[0] = '\0';
}

UILoading::~UILoading()
{
    destroy();
}

void UILoading::create(int lines, int cols)
{
    destroy();

    running = true;

    ui_window_lines = std::min(LOADING_LINES, lines);
    ui_window_cols = std::min(LOADING_COLUMNS, cols);

    window = newwin(ui_window_lines, ui_window_cols, lines / 2 - ui_window_lines / 2, cols / 2 - ui_window_cols / 2);
}

void UILoading::destroy()
{
    if (window)
    {
        delwin(window);
        window = NULL;
    }
}

void UILoading::draw()
{
    box(window, 0, 0);

    int load_win_height, load_win_width;
    getmaxyx(window, load_win_height, load_win_width);

    int loading_line = load_win_height / 2;

    wmove(window, 0, ui_window_cols / 2 - strlen(LOADING_STRING) / 2);

    wattron(window, A_REVERSE);
    waddstr(window, (const char*)LOADING_STRING);
    wattroff(window, A_REVERSE);
    wmove(window, (loading_line) - 1, 1);
    waddstr(window, "Processing: ");
    wmove(window, loading_line, 1);
    waddstr(window, "Getting list of files to check.");

    refresh();
    wrefresh(window);

    // Load once if possible to improve resize speed
    drawLoadingPath(loading_line, load_win_width);

    bool resizePending = false;

    while (running.load() && !resizePending) 
    {
        //We're currently in the loading screen
        int c = getch();

        if(c == KEY_RESIZE)
        {
            running = false;
        }
        else
        {
            // Load in the current message
            drawLoadingPath(loading_line, load_win_width);
        }
    }
}

void UILoading::stop()
{
    running = false;
}


void UILoading::drawLoadingPath(int line, int lineWidth)
{
    pathMutex.lock();

    char truncatedPath[MaxPathLen + 1] = {};

    if (pathWaiting)
    {
        wmove(window, line, 1);
        wprintw(window, "%*s", lineWidth - 2, "");

        wmove(window, line, 1);

        memcpy(truncatedPath, path, sizeof(char) *(lineWidth - 2));
        truncatedPath[lineWidth - 2] = '\0';

        waddstr(window, truncatedPath);

        wrefresh(window);
    }

    pathMutex.unlock();
}


void UILoading::updatePath(const char* p)
{
    pathMutex.lock();

    strncpy(path, p, MaxPathLen);
    path[MaxPathLen] = '\0';

    pathWaiting = true;

    pathMutex.unlock();
}