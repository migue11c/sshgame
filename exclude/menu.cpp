#include <string>
#include <vector>
#include <ncurses/ncurses.h>
#include <thread>
#include <chrono>

#include "lvlSelect.cpp"

#define TIME 30

using namespace std;

void initBox();
void drawOptions();
void cls();
void drawText();
void login();
void mainMenu();

void initBox(WINDOW* window, string text, bool skip){
    int i = 0;
    box(window,0,0);
    wrefresh(window);
    curs_set(2);
    wmove(window, 0, 3);
    while (i<text.size()){
        wprintw(window, "%c",text[i]);
        wrefresh(window);
        if (skip == false)
            this_thread::sleep_for(chrono::milliseconds(TIME));
        i++;
    }
    curs_set(0);
}

void drawOptions(WINDOW* window, vector<string> opt, bool skip){
    int i = 0;
    curs_set(2);
    while (i<size(opt)){
        wmove(window, i+2, 2);
        int j = 0;
        while (j<opt[i].size()){ //why does this hang
            wprintw(window, "%c",opt[i][j]);
            wrefresh(window);
            if (skip == false)
            	this_thread::sleep_for(chrono::milliseconds(TIME));
            j++;
        }
        i++;
    }
    curs_set(0);
}

void cls(WINDOW* window){
    int i=0;
    int x,y;
    getmaxyx(window, y, x);

    while (i<y-2){
        wmove(window, i+1, 1);
        int j = 0;
        while (j < x-1){
            wprintw(window, "");
            j++;
        }
        i++;
    }
}

//scrolling is imperfect, please look into this
void drawText(WINDOW* window, vector<string> txt, bool skip){
    int i = 0;
    int ind = 0;
    int x,y,cnt;
    curs_set(2);
    getmaxyx(window,y,x);
    while (i<size(txt) & i < y-4){
        wmove(window, ind+2, 2);
        int j = 0;
        cnt = 0;
        while (j<txt[i].size() & cnt < x-3){
            if (x-3 < txt[i].size() & cnt == x-4) {
                if(i != y-5){
                    ind++;
                    cnt = 0;
                    wmove(window, ind+2, 2);
                }
                else {
                    ind--;
                    wmove(window, 2, 2);
                    wdeleteln(window);
                    wmove(window, ind+2, ind);
                }
            }
            wprintw(window, "%c",txt[i][j]);
            wrefresh(window);
            if (skip == false)
            	this_thread::sleep_for(chrono::milliseconds(TIME));
            j++;
            cnt++;
        }
        i++;
        ind++;
    }
    curs_set(0);
}

void login() {
    drawLogin();
    if (auth()==true) {
        lvlSelect();
    }
}

void mainMenu() {
    login();
}
