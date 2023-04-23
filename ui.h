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