#include <fstream>
#include <ncurses/ncurses.h>
#include <string>
#include <cmath>
#include <cstdlib>
#include <thread>
#include <chrono>

struct vertex{
    double y,x;
};

struct map{
    vertex points[16];
};

inline int offs=0;
inline double scale=1;

void refreshTimer(bool done) {
    while (!done) {
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
        refresh();
        offs++;
    }
}

void wait() {
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
}

void textAnimation(std::string text, int y, int x){
    WINDOW* anim = newwin(1, text.length()+4, y, x);
    curs_set(2);
    std::string buffer;
    for (int i = 0;i<text.length();i++){
        wmove(anim,0, ((text.length()+4)/2) - 1 - i/2);
        buffer += text[i];
        wprintw(anim,"[%s]",buffer.c_str());
        wrefresh(anim);
        wait();
    }
    curs_set(0);
    //wclear(anim); //make sure to always wrefresh after you want to show something
    delwin(anim);
    refresh();
}

void drawVector(int sty, int stx, int finy, int finx, int div, int offs){
    int dy = abs(finy - sty);
    int sy = sty < finy ? 1 : -1;
    int dx = abs(finx - stx);
    int sx = stx < finx ? 1 : -1;
    int err = (dx > dy ? dx : -dy)/2, e2;
    int i = 0;
    while(1){
        if (sty>=0 || stx>=0 || sty<=getmaxy(stdscr) || stx<=getmaxx(stdscr)){
           	wmove(stdscr, sty, stx);
        }
        else {
            goto end;
        }
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
        end:
        i++;
    }
}

void animVector(){
    float i=0;
    bool done = false;
    std::thread rf(refreshTimer,done);
    rf.detach();
    while (1){
        clear(); //this clears entire window, need to localize it to vectors
        drawVector(7, 5, 7, 20, 3, offs/3);
        drawVector(7, 20, 17+cos(i/4)*5, 22+sin(i/4)*5, 3, offs/3);
        drawVector(17+cos(i/4)*5, 22+sin(i/4)*5, 14, 8, 3, offs/3);
        mvprintw(0, 0, "y: %f, x: %f",(17+cos(i/4)*5),(22+cos(i/4)*5));
        drawVector(14, 8, 7, 5, 3, offs/3);
        wait();
        i++;
    }
    done = true;
}

map getMap(){
    map city;
    std::ifstream in("map");
    for (int i=0;i<16;i++){
        in >> city.points[i].x >> city.points[i].y;
    }
    return city;
}

void drawCity(map city) {
    int maxy,maxx;
    getmaxyx(stdscr, maxy, maxx);
    for (int i = 0; i<16; i++){
        drawVector(scale*(city.points[i].y+20), scale*(city.points[i].x+32),
            scale*(city.points[(i+1)%16].y+20), scale*(city.points[(i+1)%16].x+32), 1, 0);
    }
}

int main(){
    map city = getMap();
    initscr();
    noecho();
    curs_set(0);
    scale = 1.4;
  //for (int i=0;i<16;i++){
  //    wmove(stdscr, i, 0);
  //    wprintw(stdscr, "x:%f, y: %f", city.points[i].x, city.points[i].y);
  //}
    drawCity(city);
    //drawVector(city.points[0].y+20, city.points[0].x+30, city.points[1].y+20, city.points[1].x+30, 1, 0);
    refresh();
    getch();
    //animVector();
    return 0;
}
