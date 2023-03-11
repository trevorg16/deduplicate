#include <iostream>
#include <curses.h>
#include <unistd.h>
#include <time.h>
#include <thread>
#include <mutex>

#define NANOS_PER_SECOND (1000000000)

const timespec noActionDelay = {.tv_sec = 0, .tv_nsec = NANOS_PER_SECOND / 1000};

std::mutex m;
int message;


void testThread()
{
    int count = 0;
    while(true)
    {
        //std::cout << "this is event: " << count << std::endl;
        m.lock();
        message = count;
        m.unlock();
        count++;
        sleep(1);
    }
}

void drawCurses()
{
    // initscr();

    // cbreak();
    // halfdelay(1);
    // noecho();
    // keypad(stdscr, TRUE);
    // int y0 = 0;
    // int x0 = 0;
    // getmaxyx(stdscr, x0, y0);

    // WINDOW *win = newwin(x0, y0,0,0);

    // // box(win, '*', '*');
    // waddstr(win, "THIS IS A TEST");
    // touchwin(win);
    // wrefresh(win);
    // refresh();
    // while(true);

    // TODO: You don't necessarily need a window, just read characters including the KEY_RESIZE

    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, true);

    int cols;
    int rows;
    getmaxyx(stdscr, rows, cols);
    printw("Hello world. Screen is (%d, %d)", rows, cols);
    refresh();
    bool running = true;

    while (running)
    {
        int c = getch();

        if (c == ERR)
        {
            nanosleep(&noActionDelay, NULL);
            continue;
        }
        
        clear();
        getmaxyx(stdscr, rows, cols);
        int count;
        m.lock();
        count = message;
        m.unlock();


        printw("getch got character %x screen is (%d, %d). message is %d", c, rows, cols, count);
        refresh();
    }

    endwin();
}

int main()
{
    std::thread t(testThread);

    drawCurses();

    // t.join();
    return 0;
}