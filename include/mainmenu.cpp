#include <ncurses/ncurses.h>
#include <fstream>
#include <string>
#include <thread>

#include "audiostuff.h"
#include "common.h"
#include "globals.h"
#include "player.h"
#include "logs.h"

void drawLogo(int ymax, int xmax) {
    std::ifstream logo("logo");
    logo.close();
}

void settingsScreen(int ymax, int xmax) {
    WINDOW* sets = newwin(10, 30, ymax/2 - 8, xmax/2 - 15);
    keypad(sets, true);
    curs_set(0);
    std::string options[3] = {"BGM Volume","SFX Volume","Save..."};
    int choice,hl;
    hl = 0;
    while(1){
        for (int i = 0; i < 3; i++) {// change with amount of options
            if (i == hl){
                mvwprintw(sets, i, 0, "> [%s]",options[i].c_str());
                wattron(sets, A_REVERSE);
            }
            else
                mvwprintw(sets, i, 0, "  [%s]",options[i].c_str());
            switch (i) {
                case 0:
                    if (clset::sets.BGMVolume == 100)
                        mvwprintw(sets, i, getmaxx(sets)-6, "[Max]");
                    else if (clset::sets.BGMVolume == 0)
                        mvwprintw(sets, i, getmaxx(sets)-6, "[Off]");
                    else
                        mvwprintw(sets, i, getmaxx(sets)-6, "[%d%%]", clset::sets.BGMVolume);
                    break;
                case 1:
                    if (clset::sets.SFXVolume == 100)
                        mvwprintw(sets, i, getmaxx(sets)-6, "[Max]");
                    else if (clset::sets.SFXVolume == 0)
                        mvwprintw(sets, i, getmaxx(sets)-6, "[Off]");
                    else
                        mvwprintw(sets, i, getmaxx(sets)-6, "[%d%%]", clset::sets.SFXVolume);
                    break;
                default:
                    break;
            }
            wattroff(sets, A_REVERSE);

            //wmove(sets, hl, options[hl].length();
            wrefresh(sets);
        }
        choice = wgetch(sets);
        std::thread press(playKey, choice);
        press.detach();

        opts:
        switch (choice) {
            case KEY_UP:{
                hl--;
                if (hl == -1)
                    hl = 0;
                break;
            }
            case KEY_DOWN:{
                hl++;
                if (hl == 3) //change this too
                    hl = 2;
                break;
            }
            case KEY_LEFT:{
                switch (hl) {
                    case 0:
                        clset::sets.BGMVolume -= 10;
                        if (clset::sets.BGMVolume < 0)
                            clset::sets.BGMVolume = 0;
                        break;
                    case 1:
                        clset::sets.SFXVolume -= 10;
                        if (clset::sets.SFXVolume < 0)
                            clset::sets.SFXVolume = 0;
                        break;
                    default:
                        break;
                }
                break;
            }
            case KEY_RIGHT:{
                switch (hl) {
                    case 0:
                        clset::sets.BGMVolume += 10;
                        if (clset::sets.BGMVolume > 100)
                            clset::sets.BGMVolume = 100;
                        break;
                    case 1:
                        clset::sets.SFXVolume += 10;
                        if (clset::sets.SFXVolume > 100)
                            clset::sets.SFXVolume = 100;
                        break;
                    default:
                        break;
                }
                break;
            }
            default: if (choice == 10 && hl == 2) {goto end;} //this goes to end regardless of the result
            //you need to write a way to do settings within the function
        }
    }
    end:
    setSets();
    wclear(sets);
    clear();
    delwin(sets); //make sure to always refresh() after delwin and not wrefresh(win)!!!
    refresh();
}

int mainScreenOptions(int ymax, int xmax){
    WINDOW* menu = newwin(3, 20, 5*ymax/6, xmax/8);
    keypad(menu, true);
    std::string options[3] = {"Login", "Settings", "Exit"};
    int choice,hl;
    hl = 0;
    while(1){
        for (int i = 0; i < 3; i++){
            if (i == hl) {
                wattron(menu, A_REVERSE);
                //mvwprintw(menu, i, 0, "[%s]",options[i].c_str());
            }
            mvwprintw(menu, i, 0, "[%s]",options[i].c_str());
            wattroff(menu, A_REVERSE);
            wrefresh(menu);
        }
        choice = wgetch(menu);
        std::thread press(playKey, choice);
        press.detach();

        switch (choice){
            case KEY_UP:
                hl--;
                if (hl == -1)
                    hl = 0;
                break;
            case KEY_DOWN:
                hl++;
                if (hl == 3)
                    hl = 2;
                break;
            default:
                break;
        }
        if (choice == 10){
            break;
        }
    }
    wclear(menu);
    clear();
    delwin(menu);
    refresh();
    return hl;
}

void drawUser(){
    WINDOW* user = newwin(6, 30, 0, 0);
    refresh();
    for (int i = 0; i<5;i++){
        switch (i) {
            case 0:
                wmove(user, 0, 0);
                wprintw(user, "Username: %s",player::mydata.name);
                break;
            case 1:
                wmove(user, 1, 0);
                //wprintw(user, "Faction: %s",player::mydata.faction.c_str());
                break;
            case 2:
                wmove(user, 2, 0);
                wprintw(user, "Max HP: %d",player::mydata.maxhp);
                break;
            case 3:
                wmove(user, 3, 0);
                wprintw(user, "Current HP: %d",player::mydata.hp);
                break;
            case 4:
                wmove(user, 4, 0);
                wprintw(user, "Dice:");
                for (int j = 0; j < 5; j++){
                    wprintw(user, " %d",player::mydata.dice[j]);
                }
            case 5:
                wmove(user, 5, 0);
                wprintw(user,"Items:");
                for (int j = 0; j < 100; j++){
                    wprintw(user, " %d",player::mydata.items[j]);
                }
            default:
                break;
        }
    }
    wrefresh(user);
    LogThis("drawn userdata");
}
