#include <algorithm>
#include <cstring>
#include <curses.h>

#include "ui_loading.h"

#define LOADING_LINES (8)
#define LOADING_COLUMNS (64)
#define LOADING_STRING "LOADING"

UILoading::UILoading() : 
    // running(true),
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
    //mvwaddstr(loading_window, 0,0,"");
    refresh();
    wrefresh(window);

    // Load once if possible to improve resize speed
    drawLoadingPath(loading_line, load_win_width);

    bool resizePending = false;

    while (running && !resizePending) 
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