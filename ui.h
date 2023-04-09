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

// class ui_location {
//     public:
//         DuplicateFinderPath::PathOrderedList_t::size_type main_entry;
//         DuplicateFinderPath::PathList::size_type sub_entry;
//         bool operator> (const ui_location &b){
//             if (this->main_entry != b.main_entry){
//                 return this->main_entry > b.main_entry;
//             }
//             else{
//                 return this->sub_entry > b.sub_entry;
//             }
//         }
//         bool operator< (const ui_location &b){
//             if (this->main_entry != b.main_entry){
//                 return this->main_entry < b.main_entry;
//             }
//             else{
//                 return this->sub_entry < b.sub_entry;
//             }
//         }
// };

// struct UIState
// {
//     UIState() :
//         ui_screen(UiScreenStates_Main),
//         // nlines(0),
//         // ncols(0),
//         // currently_running(true),
//         loadingWindow(NULL),
//         x_scroll(0),
//         top_line({}),
//         bottom_line({}),
//         select_line({})
//     {
//     }

//     uint8_t ui_screen;
//     // int nlines;
//     // int ncols;
//     // bool currently_running;
//     // WINDOW* ui_windows[UiScreenNumStates];
//     WINDOW* loadingWindow;
//     int x_scroll;
//     UILocation top_line;
//     UILocation bottom_line;
//     UILocation select_line;
// };

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