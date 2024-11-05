#include <SFML/Audio/Music.hpp>
#include <chrono>
#include <csignal>
#include <ncurses/ncurses.h>
#include <string>
#include <thread>
#include <SFML/Audio.hpp>
#include <SFML/System/Time.hpp>
//#include "include/mainmenu.cpp"
//#include "include/map.cpp"
//#include "include/login.cpp"
//#include "include/logs.cpp"
#include "include/audiostuff.h"
#include "include/common.h"
#include "include/globals.h"
#include "include/logs.h"
#include "include/player.h"

int main(){
    // exit lambda func
    auto lam = [] (int i) {gameaudio::kill = true; printw("RAHHH"); wrefresh(stdscr); std::this_thread::sleep_for(std::chrono::milliseconds(100)); exit(0); };
    //^C
    signal(SIGINT, lam);
    //abort()
    signal(SIGABRT, lam);
    //sent by "kill" command
    signal(SIGTERM, lam);
    //^Z
    //signal(SIGTSTP, lam);

    //initializing
    int y,x,yMax,xMax;
    LogThis("made it compile");
    getSets();
    loadAudio();
    std::thread musicplayer(playMusic);
    musicplayer.detach();

    initscr();
    getmaxyx(stdscr,yMax,xMax);
    noecho();
    cbreak();
    curs_set(0);
    //animVector();
    //draw intro animation here

    //animVector();
    //wrefresh(stdscr);
    //getch();
    start:
    switch (mainScreenOptions(yMax, xMax)) {
        case 0: { //get login
            if (login(yMax,xMax) != true){
                goto start;
            }
            break;
        }
        case 1: { //settings
            settingsScreen(yMax,xMax); //unfinished, for now only returns you here
            goto start;
        }
        case 2: { //exit the game
            SIGINT;
        }
        default: { //exits the game with error
            refresh();
            return 1;
        }
    }
    gameaudio::kill = true;
    LogThis(player::mydata.name);
    LogThis(player::mydata.faction);
    drawUser();
    textAnimation(player::mydata.faction,10,xMax/2 - (player::mydata.faction.length()+4)/2);
    //just so it's sitting still
    getch();
    clear();
    refresh();
    endwin();
    setSets();
    dumpAudio();
    return 0;
}
