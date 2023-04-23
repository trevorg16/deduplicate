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

#ifndef UI_LOADING_HH
#define UI_LOADING_HH

#include <mutex>
#include <atomic>
#include <curses.h>

const int MaxPathLen = 256;

class UILoading
{
    public:
        UILoading();
        ~UILoading();
        void create(int lines, int cols);
        void destroy();
        void draw();
        void stop();
        void drawLoadingPath(int line, int lineWidth);
        void updatePath(const char* p);
    private:
        unsigned int ui_window_lines;
        unsigned int ui_window_cols;

        std::atomic<bool> running;

        WINDOW* window;

        char path[MaxPathLen + 1];
        bool pathWaiting;
        std::mutex pathMutex;
};

#endif /* UI_LOADING_HH */