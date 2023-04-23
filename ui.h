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

#ifndef UI_HH
#define UI_HH

#include "duplicate_finder.h"
#include <curses.h>
#include <mutex>

#include "ui_loading.h"
#include "ui_data.h"

// const int MaxPathLen = 256;

enum UiScreenStates
{
    UiScreenStates_Main = 1,
    UiScreenStates_Loading = 2,
    UiScreenStates_Data = 4
};

const int UiScreenNumStates = 3;

class UI
{
    public:
    UI();
    ~UI();
    void init();
    void run();
    void finderComplete(DuplicateFinder* finder);
    void updatePath(const char* p);

    private:
    void drawScreenMain();
    void drawScreen();

    // Move to class storing bath and duplicate info

    DuplicateFinder* duplicateFinder;
    std::mutex duplicateFinderMutex;

    bool running;

    uint8_t ui_screen;

    UILoading loadingScreen;
    UIData    dataScreen;
};

#endif /*UI_HH */