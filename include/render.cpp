#include <ncurses/ncurses.h>
#include <string>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <thread>
#include <chrono>

#include "render.h"
#include "common.h"
#include "globals.h"
#include "logs.h"

// TODO: scanline floodfill algorithm https://lodev.org/cgtutor/floodfill.html
//          This is so that you (likely) don't have to implement drawing each polygon.
//          Good idea is to use mvwinchstr()
// Anchored camera view (pointed camera is done, i just need to make an anchored one and calculate a midpoint)
// Unified system of loading vertices and returning a vector of vectors (i fucking hate and love this)
// Point of interest additional information
// draw and move between different sets of POIs based on scale
// add POI level, so that not all are selectable
// add hidden POIs that can't be interacted with unless a condition is satisfied
// change camera anchor based on sector and scale
// create a menu to exit the game or enter the settings page

//shell and map is redundant, please implement an array of walls.

void wait() {
    std::this_thread::sleep_for(std::chrono::milliseconds(25));
}

void textAnimation(WINDOW* win, std::string text, int len, int y, int x, int offs){
    WINDOW* anim = newwin(1, len+3, y+offs, x+offs);
    //curs_set(2);
    std::string buffer;
    for (int i = 0;i<=len;i++){
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

char dspchar(int sy, int sx, bool mvy, bool mvx){
    if (mvy != mvx) {
        if (mvy) {
            return '|';
        }
        else {
            return '-';
        }
    }

    if (sy == 1 && sx == 1 || sy == -1 && sx == -1) {
        return '\\';
    }
    else {
        return '/';
    }
    // '/', '\\', '|', '-'
}

void drawVector(WINDOW* win, int sty, int stx, int finy, int finx, int div, int offs){
    int dy = abs(finy - sty);
    int sy = sty < finy ? 1 : -1;
    int dx = abs(finx - stx);
    int sx = stx < finx ? 1 : -1;
    int err = (dx > dy ? dx : -dy)/2, e2;
    bool mvx, mvy;
    int i = 0;
    while(1){
        wmove(win, sty, stx);
        if (sty>=0 && stx>=0 && sty<getmaxy(win) && stx<getmaxx(win) && (i+offs)%div == 0){
            wprintw(win, "%c", dspchar(sy,sx,mvy,mvx));
        }

        if (stx==finx && sty==finy){
            break;
        }
        mvx = false;
        mvy = false;
        e2=err;

        if (e2 > -dx){
            err -= dy;
            stx += sx;
            mvx = true;
        }

        if (e2 < dy){
            err += dx;
            sty += sy;
            mvy = true;
        }
        // end: why did i do this again?
        //
        i++;
    }
}

shell getShell(){ // temporary
    shell dist;
    std::ifstream in("resources//walls");
    if (!in.bad()){
        LogThis("good walls dir");
        for (int i=0;i<70;i++){
            in >> dist.points[i].x >> dist.points[i].y;
        }
    }
    else {
        LogThis("bad walls dir");
    }
    in.close();
    return dist;
}

void drawShell(WINDOW* win, shell dist, double scale){ //essentially drawCity, just different for loop amount
    float maxy,maxx;
    getmaxyx(win, maxy, maxx);
    for (int i = 0; i<69; i++){ //draws each segment to scale and with camera
        if (maxy/2+scale*(dist.points[i].y-cpos.y) >= 0 || maxx/2+scale*(dist.points[i].x-cpos.x) >= 0 ||
            maxy/2+scale*(dist.points[(i+1)].y-cpos.y) < maxy || maxx/2+scale*(dist.points[(i+1)].x-cpos.x) < maxx){
                drawVector(win,maxy/2+scale*(dist.points[i].y-cpos.y), maxx/2+scale*(dist.points[i].x-cpos.x),
                    maxy/2+scale*(dist.points[(i+1)].y-cpos.y), maxx/2+scale*(dist.points[(i+1)].x-cpos.x), 1, 0);
        }
    }
}

map getMap(){
    map city;
    std::ifstream in("resources//map");
    if (!in.bad()){
        LogThis("good map");
        for (int i=0;i<30;i++){
            in >> city.points[i].x >> city.points[i].y;
        }
    }
    else {
        LogThis("bad map");
    }
    in.close();
    return city;
}

void drawCity(WINDOW* win, map city, double scale) {
    float maxy,maxx;
    getmaxyx(win, maxy, maxx);

    for (int i = 0; i<30; i++){ //draws each segment to scale and with camera
        if (maxy/2+scale*(city.points[i].y-cpos.y) >=0 || maxx/2+scale*(city.points[i].x-cpos.x >= 0) ||
            maxy/2+scale*(city.points[(i+1)%30].y-cpos.y) < maxy || maxx/2+scale*(city.points[(i+1)%30].x-cpos.x) < maxx){
                drawVector(win, maxy/2+scale*(city.points[i].y-cpos.y), maxx/2+scale*(city.points[i].x-cpos.x),
                    maxy/2+scale*(city.points[(i+1)%30].y-cpos.y), maxx/2+scale*(city.points[(i+1)%30].x-cpos.x), 1, 0);
            }
    }
}

void drawPos(vertex cam, double scale){ // debugging
    mvwprintw(stdscr,3,3, "x = %f; y = %f; scale = %f",cam.x,cam.y,scale);
}

double distance(vertex st, vertex en){ // logic
    return std::sqrt((en.x-st.x)*(en.x-st.x)+(en.y-st.y)*(en.y-st.y));
}

std::vector<poi> getPoi(){ // point of interest input
    std::vector<poi> Poi;
    std::ifstream file("resources//map.dat");
    int i = 0;
    if (!file.bad()){
        LogThis("good map.dat");
        while (i<25){
            poi buffer; // this buffer is reinitialized every time instead of just being cleared but it's fine cuz it's easier. Bother if you must.
            file >> buffer.coord.y >> buffer.coord.x >> buffer.cond >> buffer.scl; // gets y, x and condition in 1 line.
            buffer.len=0; // initializes text length
            file.get(buffer.name[buffer.len]); // for some reason this needs to stay after a float
            while (buffer.len<24){ // 24 characters
                file.get(buffer.name[buffer.len]); // gets each character
                if (buffer.name[buffer.len] == '\n'){
                    buffer.name[buffer.len] = 0;
                    break;
                }
                buffer.len++;
            }
            Poi.push_back(buffer);
            i++;
        }
    }
    else {
        LogThis("bad map.dat");
    }
    file.close();
    return Poi;
}

void drawPoi(WINDOW* win, std::vector<poi> poi,double scale,int offs){
    float maxx,maxy;
    int mark;
    getmaxyx(win, maxy, maxx);
    for (int i=0; i<poi.size(); i++){
        if (cpos.x != poi[i].coord.x && cpos.y != poi[i].coord.y &&
            maxy/2+scale*(poi[i].coord.y-cpos.y) >= 0 && maxy/2+scale*(poi[i].coord.y-cpos.y)<maxy &&
            maxx/2+scale*(poi[i].coord.x-cpos.x) >= 1 && maxx/2+scale*(poi[i].coord.x-cpos.x)<maxx-1){
            mvwprintw(win, maxy/2+scale*(poi[i].coord.y-cpos.y), maxx/2+scale*(poi[i].coord.x-cpos.x)-1, "[x]");
        }
        else if (cpos.x == poi[i].coord.x && cpos.y == poi[i].coord.y){
            mark = i;
        }
    }
    wrefresh(win);
    if (scale>=0.25) {
        textAnimation(win, poi[mark].name,poi[mark].len, maxy/2, (maxx/2)-2-poi[mark].len/2,offs); // the +2 needs to be fixed
    }
}
