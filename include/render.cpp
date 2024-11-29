#include <ncurses/ncurses.h>
#include <string>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <thread>
#include <chrono>

#include "common.h"

static vertex cpos = {0,0}; //THIS IS TEMPORARY AND NEEDS TO BE INITIALIZED BY SECTOR

struct shell {
    vertex points[70];
};

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

void drawVector(WINDOW* win,int sty, int stx, int finy, int finx, int div, int offs){
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

shell getShell(){ // temporary
    shell dist;
    std::ifstream in("walls");
    for (int i=0;i<70;i++){
        in >> dist.points[i].x >> dist.points[i].y;
    }
    return dist;
}

void drawShell(WINDOW* win, shell dist, double scale){
    float maxy,maxx;
    getmaxyx(win, maxy, maxx);
    for (int i = 0; i<69; i++){ //draws each segment to scale and with camera
        if (maxy/2+scale*(dist.points[i].y-cpos.y) >= 0 || maxx/2+scale*(dist.points[i].x-cpos.x) >= 0 ||
                maxy/2+scale*(dist.points[(i+1)].y-cpos.y) < maxy || maxx/2+scale*(dist.points[(i+1)].x-cpos.x) < maxx){
        }
        drawVector(win,maxy/2+scale*(dist.points[i].y-cpos.y), maxx/2+scale*(dist.points[i].x-cpos.x),
            maxy/2+scale*(dist.points[(i+1)].y-cpos.y), maxx/2+scale*(dist.points[(i+1)].x-cpos.x), 1, 0);
    }
  //for (int i = 0; i<70;i++){ // this needs to be removed on release
  //    mvprintw(maxy/2+scale*(dist.points[i].y-cpos.y), maxx/2+scale*(dist.points[i].x-cpos.x), "%d",i+1);
  //}
}
