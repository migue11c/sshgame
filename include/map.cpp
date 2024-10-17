#include <fstream>
#include <string>
#include <ncurses/ncurses.h>

#include "globals.h"

using namespace std;

void initMap() {
    WINDOW* mapDisplay = newwin(50, 50, 15, 20);
}

void drawMap(WINDOW* win, mapDetails map) {
    int i=0;
    int ind=0;
    int x,y,cnt;
    string input;
    curs_set(2);
    getmaxyx(win, y, x);
    string mapdir = "maps\\" + map.name;
    ifstream buffer(mapdir);
    while (i < y-4){
        getline(buffer,input);
        wmove(win, i+2, 2);
        int j = 0;
        cnt = 0;
        while (j < x-4){
            wprintw(win, "%c",input[j+30]);
            j++;
        }
        i++;
    }
    wrefresh(win);
    curs_set(0);
    buffer.close();
}
