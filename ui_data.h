#ifndef UI_DATA_HH
#define UI_DATA_HH

#define _FILE_OFFSET_BITS 64

#include <mutex>
#include <atomic>
#include <curses.h>

#include "duplicate_finder.h"

// struct UILocation {
//     public:
//         FileInfoKey mainEntry;
//         FilenameList::size_type subEntry;
// };

struct UILocation
{
    // PathMap::iterator mainEntry;
    PathMap::reverse_iterator mainEntry;
    FilenameList::size_type subEntry;

    bool operator> (const UILocation& b) const {
        const UILocation& a = *this;

        if (a.mainEntry != b.mainEntry){
            return FileInfoKey::lessThan(a.mainEntry->first, b.mainEntry->first);
        }
        else{
            return a.subEntry > b.subEntry;
        }
    }

    bool operator< (const UILocation &b){
        const UILocation& a = *this;

        if (this->mainEntry != b.mainEntry){
            return FileInfoKey::greaterThan(a.mainEntry->first, b.mainEntry->first);
        }
        else{
            return this->subEntry < b.subEntry;
        }
    }

    // bool operator== (const UILocation &b)
    // {
    //     return this->mainEntry == b.mainEntry && this->subEntry == b.subEntry;
    // }
};
// struct UILocation
// {
// PathMap::iterator mainEntry;
// FilenameList::size_type subEntry;
// };

class UIData
{
    public:

    UIData();

    ~UIData();

    void create(int lines, int cols);

    void destroy();

    void draw();

    void updateData(PathMap* map);

    bool didQuit();

    private:

    void scrollUp();

    void shiftDown(UILocation& location);

    void shiftUp(UILocation& location);

    void scrollDown();

    void drawScreen(PathMap* map);

    static void byteSizeToStr(off_t byteSize, char* dest, size_t maxStringLen);

    WINDOW* window;

    PathMap* pathMap;

    std::mutex pathMapMutex;

    int x_scroll;

    UILocation top_line;
    UILocation bottom_line;
    UILocation select_line;

    bool firstRun;

    bool running;
    bool quit;

    int nlines;
    int ncols;
};

#endif /* UI_DATA_HH */