#include "ui.h"
#include <signal.h>
#include <cstring>

#define PROGRAM_NAME "Deduplicator"

#define mvaddstrfill(y, x, l, s) mvaddstr(y, x, s); printw("%*s", l - strlen(s), "")

UI::UI() : 
    duplicateFinder(NULL),
    duplicateFinderMutex(),
    running(true),
    ui_screen(UiScreenStates_Main | UiScreenStates_Loading),
    x_scroll(0),
    top_line({}),
    bottom_line({}),
    select_line({})
{
    init();
}

void UI::init()
{
    initscr();
    cbreak();
    halfdelay(1);
    noecho();
    keypad(stdscr, TRUE);
}

void UI::drawScreenMain()
{
    int nlines;
    int ncols;

    getmaxyx(stdscr, nlines, ncols);

    attron(A_REVERSE);
    mvaddstrfill(0, 0, ncols, PROGRAM_NAME);
    attroff(A_REVERSE);

    mvprintw(1, 0, "Current screen size: (%d, %d), pid: %d", ncols, nlines, (int)getpid());
}

void UI::drawScreen()
{
    drawScreenMain();

    if (ui_screen & UiScreenStates_Loading)
    {
        int nlines = 0;
        int ncols = 0;

        getmaxyx(stdscr, nlines, ncols);
        loading.create(nlines, ncols);
        loading.draw();
    }
    else if(true)
    {

    }
}

void UI::run()
{
    while(running)
    {
        clear();
        

        drawScreen();
    }
}

void UI::finderComplete(const DuplicateFinder* finder)
{
    duplicateFinderMutex.lock();

    duplicateFinder = finder;

    duplicateFinderMutex.unlock();
}

void UI::updatePath(const char* p)
{
    loading.updatePath(p);
}