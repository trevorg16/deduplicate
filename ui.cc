#include "ui.h"
#include <signal.h>
#include <cstring>

#define PROGRAM_NAME "Deduplicator"

#define mvaddstrfill(y, x, l, s) mvaddstr(y, x, s); printw("%*s", l - strlen(s), "")

#define LOADING_LINES (8)
#define LOADING_COLUMNS (64)
#define LOADING_STRING "LOADING"

UI::UI() : 
    pathWaiting(false),
    pathMutex(),
    duplicateFinder(NULL),
    duplicateFinderMutex()
{
    path[0] = '\0';
    init();
}

bool UI::init()
{
    initscr();
    cbreak();
    halfdelay(1);
    noecho();
    keypad(stdscr, TRUE);

    uiState.ui_screen |= UiScreenStates_Loading;
    return true;
}

void deleteAllWindow()
{

}

void UI::runLoading()
{

    // bool reloadPending = false;

    // getmaxyx(stdscr, ui_state.nlines, ui_state.ncols);
    unsigned int ui_window_lines = std::min(LOADING_LINES, uiState.nlines);
    unsigned int ui_window_cols = std::min(LOADING_COLUMNS, uiState.ncols);


    // initscr();
    // clear();
    // deleteAllWindows();
    // drawScreen();
    
    if (uiState.loadingWindow)
    {
        delwin(uiState.loadingWindow);
        uiState.loadingWindow = NULL;
    }

    uiState.loadingWindow = newwin(ui_window_lines, ui_window_cols, uiState.nlines / 2 - ui_window_lines / 2, uiState.ncols / 2 - ui_window_cols / 2);
    
    box(uiState.loadingWindow, 0, 0);

    int load_win_height, load_win_width;
    getmaxyx(uiState.loadingWindow, load_win_height, load_win_width);

    int loading_line = load_win_height / 2;

    wmove(uiState.loadingWindow, 0, ui_window_cols / 2 - strlen(LOADING_STRING) / 2);

    wattron(uiState.loadingWindow, A_REVERSE);
    waddstr(uiState.loadingWindow, (const char*)LOADING_STRING);
    wattroff(uiState.loadingWindow, A_REVERSE);
    wmove(uiState.loadingWindow, (loading_line) - 1, 1);
    waddstr(uiState.loadingWindow, "Processing: ");
    wmove(uiState.loadingWindow, loading_line, 1);
    waddstr(uiState.loadingWindow, "Getting list of files to check.");
    //mvwaddstr(loading_window, 0,0,"");
    refresh();
    wrefresh(uiState.loadingWindow);

    // Load once if possible to improve resize speed
    drawLoadingPath(loading_line, load_win_width);

    bool needResize = false;

    while(uiState.ui_screen & UiScreenStates_Loading && !needResize){
        //We're currently in the loading screen
        int c = getch();

        if(c == KEY_RESIZE)
        {
            needResize = true;
        }
        else
        {
            // Load in the current message
            drawLoadingPath(loading_line, load_win_width);
        }

        // ui_message_t loaded_message;
        // ssize_t readLen = read(messagePipe[0], &loaded_message, sizeof(ui_message_t));

        // if(readLen == -1)
        // {
        //     if (errno == EINTR)
        //     {
        //         // Assume resize
        //         reloadPending = true;

        //         continue;
        //     }
        //     perror("Failed to read from pipe\n");
        //     exit(EXIT_FAILURE);
        // }
        // else if (readLen < sizeof(ui_message_t))
        // {
        //     // Incomplete read, skip
        //     continue;
        // }
        // Good read

        // {
        //     switch(loaded_message.message_type){
        //         case UI_MESSAGE_EMPTY:
        //             break;
        //         case UI_MESSAGE_TYPE_NEW_PATH:
        //             {
        //                 //Draw the new path
        //                 //Clear the line
        //                 wmove(loading_window, loading_line, 1);
        //                 wprintw(loading_window, "%*s", load_win_width - 2, "");
        //                 wmove(loading_window, loading_line, 1);
        //                 loaded_message.message[load_win_width - 2] = '\0';
        //                 waddstr(loading_window, loaded_message.message);
        //                 wrefresh(loading_window);
        //             }
        //             break;
        //         case UI_MESSAGE_TYPE_RESIZE:
        //             {
        //                 getmaxyx(stdscr, ui_state.nlines, ui_state.ncols);
        //                 reloadPending = true;
        //             }
        //             break;
        //         case UI_MESSAGE_TYPE_LOADING_COMPLETE:
        //             {
        //                 ui_state.ui_screen &= ~LOADING;
        //                 delwin(loading_window);
        //                 runUI();
        //             }
        //             break;
        //     }
        // }
    }
}

void UI::drawScreenMain()
{
    attron(A_REVERSE);
    mvaddstrfill(0, 0, uiState.ncols, PROGRAM_NAME);
    attroff(A_REVERSE);
    mvprintw(1, 0, "Current screen size: (%d, %d), pid: %d", uiState.ncols, uiState.nlines, (int)getpid());
}

void UI::drawLoadingPath(int line, int lineWidth)
{
    pathMutex.lock();

    char truncatedPath[MaxPathLen + 1] = {};

    if (pathWaiting)
    {
        wmove(uiState.loadingWindow, line, 1);
        wprintw(uiState.loadingWindow, "%*s", lineWidth - 2, "");
        
        wmove(uiState.loadingWindow, line, 1);

        memcpy(truncatedPath, path, sizeof(char) *(lineWidth - 2));
        truncatedPath[lineWidth - 2] = '\0';
        
        waddstr(uiState.loadingWindow, truncatedPath);

        wrefresh(uiState.loadingWindow);
    }

    pathMutex.unlock();
}

void UI::drawScreen()
{
    if (uiState.ui_screen & UiScreenStates_Loading)
    {
        runLoading();
    }
    else if(true)
    {

    }
}

bool UI::run()
{
    while(true)
    {
        clear();
        getmaxyx(stdscr, uiState.nlines, uiState.ncols);
        drawScreenMain();
        drawScreen();
    }

    return true;
}

void UI::finderComplete(const DuplicateFinder* finder)
{
    duplicateFinderMutex.lock();

    duplicateFinder = finder;

    duplicateFinderMutex.unlock();
}

void UI::updatePath(const char* p)
{
    pathMutex.lock();

    strncpy(path, p, MaxPathLen);
    path[MaxPathLen] = '\0';

    pathWaiting = true;

    pathMutex.unlock();
}