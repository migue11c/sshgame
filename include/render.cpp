#include <ncurses/ncurses.h>
#include <string>
#include <cmath>
#include <cstdlib>
#include <thread>
#include <chrono>

#include "common.h"


void wait() {
    std::this_thread::sleep_for(std::chrono::milliseconds(25));
}

void textAnimation(WINDOW* win, std::string text, int len, int y, int x, int offs){
    WINDOW* anim = newwin(1, text.length()+4, y, x);
    //curs_set(2);
    std::string buffer;
    for (int i = 0;i<text.length();i++){
        wmove(anim,0, ((len+4)/2) - 1 - i/2);
        buffer += text[i];
        if (x>=0 && y>=0 && x<getmaxx(win) && y<getmaxy(win)){
            wprintw(anim,"[%s]",buffer.c_str());
        }
        wrefresh(anim);
        wait();
    }
    //curs_set(0);
    //wclear(anim); //make sure to always wrefresh after you want to show something
    delwin(anim);
    //refresh();
}

void drawVector(int sty, int stx, int finy, int finx, int div, int offs){
    int dy = abs(finy - sty);
    int sy = sty < finy ? 1 : -1;
    int dx = abs(finx - stx);
    int sx = stx < finx ? 1 : -1;
    int err = (dx > dy ? dx : -dy)/2, e2;
    int i = 0;
    while(1){
    	wmove(stdscr, sty, stx);
        if ((i+offs)%div == 0){
            wprintw(stdscr, "#");
        }

        if (stx==finx && sty==finy){
            break;
        }

        e2=err;

        if (e2 > -dx){
            err -= dy;
            stx += sx;
        }

        if (e2 < dy){
            err += dx;
            sty += sy;
        }
        i++;
    }
}
