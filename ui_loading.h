#ifndef UI_LOADING_HH
#define UI_LOADING_HH

#include <mutex>
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
        // int width;
        // int height;
        unsigned int ui_window_lines;
        unsigned int ui_window_cols;

        bool running;

        WINDOW* window;

        char path[MaxPathLen + 1];
        bool pathWaiting;
        std::mutex pathMutex;
};

#endif /* UI_LOADING_HH */