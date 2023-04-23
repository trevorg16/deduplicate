#include "ui.h"
#include <signal.h>
#include <cstring>

#define PROGRAM_NAME "Deduplicator"

#define mvaddstrfill(y, x, l, s) mvaddstr(y, x, s); printw("%*s", static_cast<int>(l - strlen(s)), "")

UI::UI() :
    duplicateFinder(NULL),
    duplicateFinderMutex(),
    running(true),
    ui_screen(UiScreenStates_Main | UiScreenStates_Loading)
{
    init();
}

UI::~UI()
{
    loadingScreen.destroy();
    dataScreen.destroy();
    endwin();
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
    int ncols = getmaxx(stdscr);

    attron(A_REVERSE);
    mvaddstrfill(0, 0, ncols, PROGRAM_NAME);
    attroff(A_REVERSE);
}

void UI::drawScreen()
{
    int nlines = 0;
    int ncols = 0;

    getmaxyx(stdscr, nlines, ncols);

    duplicateFinderMutex.lock();

    if (!(ui_screen & UiScreenStates_Data) && duplicateFinder != NULL)
    {
        ui_screen = UiScreenStates_Main | UiScreenStates_Data;

        dataScreen.updateData(duplicateFinder->getFileMap());
    }

    duplicateFinderMutex.unlock();
    
    drawScreenMain();

    if (ui_screen & UiScreenStates_Loading)
    {
        loadingScreen.create(nlines, ncols);
        loadingScreen.draw();
    }
    else if(ui_screen & UiScreenStates_Data)
    {
        dataScreen.create(nlines, ncols);
        dataScreen.draw();
    }
}

void UI::run()
{
    while(running && !dataScreen.didQuit())
    {
        clear();

        drawScreen();
    }
}

void UI::finderComplete(DuplicateFinder* finder)
{
    duplicateFinderMutex.lock();

    duplicateFinder = finder;

    duplicateFinderMutex.unlock();

    loadingScreen.stop();
}

void UI::updatePath(const char* p)
{
    loadingScreen.updatePath(p);
}