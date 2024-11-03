#include <ncurses/ncurses.h>
#include <string>
#include <cmath>
#include <cstdlib>
#include <thread>
#include <chrono>

inline int offs=0;

void refreshTimer() {
    while (1) {
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

void animVector(){
    float i=0;
    std::thread rf(refreshTimer);
    rf.detach();
    while (1){
        std::chrono::steady_clock clock;
        clear();
        drawVector(7, 5, 7, 20, 3, offs/3);
        drawVector(7, 20, 17+cos(i/4)*5, 22+sin(i/4)*5, 3, offs/3);
        drawVector(17+cos(i/4)*5, 22+sin(i/4)*5, 14, 8, 3, offs/3);
        mvprintw(0, 0, "y: %f, x: %f",(17+cos(i/4)*5),(22+cos(i/4)*5));
        drawVector(14, 8, 7, 5, 3, offs/3);
        wait();
        i++;
    }
}

int main(){
    initscr();
    noecho();
    curs_set(0);
    animVector();
    return 0;
}
