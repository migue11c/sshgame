#include <ncurses/ncurses.h>
#include <algorithm>
#include <fstream>
#include <string>
#include <thread>

#include "audiostuff.h"
#include "render.h"
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

void cityrender(){
    // right now all nodes render regardless of poi[i].cond
    WINDOW* border = newwin(getmaxy(stdscr)-2, getmaxx(stdscr)-2, 1, 1);
    WINDOW* worldmap = newwin(getmaxy(stdscr)-4, getmaxx(stdscr)-4, 2, 2);
    shell dist = getShell();

    int offs = 2; // this actually needs to be implemented because some things are not displayed relative to the center but to the corner

    std::vector<poi> poi = getPoi();
    map city = getMap();
    keypad(worldmap, true);
    int sc=0;
    double scale = 0.2;
    cpos = poi[0].coord;
    float maxx,maxy;
    getmaxyx(worldmap, maxy, maxx);
    box(border, 0, 0);
    mvwprintw(border, 0, 3, "[World Map]");
    wrefresh(border);
    while(1){ // the animations here are linear. find a way to add a bezier curve to them
        wclear(worldmap);
        drawCity(worldmap,city,scale);
        if (scale > 0.25){
            drawShell(worldmap, dist, scale);
        }
        drawPos(cpos, scale);
        //drawVector(worldmap, maxy, maxx/2-maxy, 0, maxx/2+maxy, 4, 0); // this is direction testing
        //drawVector(worldmap, 0, maxx/2-maxy, maxy, maxx/2+maxy, 4, 0);
        wrefresh(worldmap);
        vertex cit = {86,210};

        if (scale <= 0.25){ // make this a new thread
            textAnimation(worldmap, "The City",8, maxy/2+scale*(cit.y-cpos.y), maxx/2+scale*(cit.x-cpos.x)-4, offs); // THIS IS NOT RELATIVE TO A WINDOW
        }
        else {
            drawPoi(worldmap,poi, scale,offs);
        }
        // this manually moves the camera by pixels (this will be useful for dungeons but not for this)
        ch:
        switch (wgetch(worldmap)) { // please work on the dist method. it doesn't work properly
            case KEY_UP:{
                std::vector<double> dist;
                for (int i=0; i<poi.size();i++){
                    if (2*(poi[i].coord.y-cpos.y) <= -std::abs(poi[i].coord.x-cpos.x) && // vertical
                        poi[i].coord.y-cpos.y != 0){ // it goes up
                            dist.push_back(distance(cpos, poi[i].coord));
                    }
                    else{
                        dist.push_back(99999);
                    }
                  //mvwprintw(stdscr, i, 0, "%d, %f",i,dist[i]);
                }
                int it = std::distance(dist.begin(),std::min_element(dist.begin(),dist.end()));
                if (dist[it] != 99999){
                    cpos = poi[it].coord;
                }
                else{
                    goto ch;
                }
              //getch();
                break;
            }
            case KEY_DOWN:{
                std::vector<double> dist;
                for (int i=0; i<poi.size();i++){
                    if (2*(poi[i].coord.y-cpos.y) >= std::abs(poi[i].coord.x-cpos.x) && // vertical
                        poi[i].coord.y-cpos.y != 0){ // it goes up
                            dist.push_back(distance(cpos, poi[i].coord));
                    }
                    else{
                        dist.push_back(99999);
                    }
                  //mvwprintw(stdscr, i, 0, "%d, %f",i,dist[i]);
                }
                int it = std::distance(dist.begin(),std::min_element(dist.begin(),dist.end()));
                if (dist[it] != 99999){
                    cpos = poi[it].coord;
                }
                else{
                    goto ch;
                }
              //getch();
                break;
            }
            case KEY_LEFT:{
                std::vector<double> dist;
                for (int i=0; i<poi.size();i++){
                    if (poi[i].coord.x-cpos.x <= -2*std::abs(poi[i].coord.y-cpos.y) && // vertical
                        poi[i].coord.x-cpos.x != 0){ // it goes up
                            dist.push_back(distance(cpos, poi[i].coord));
                    }
                    else{
                        dist.push_back(99999);
                    }
                  //mvwprintw(stdscr, i, 0, "%d, %f",i,dist[i]);
                }
                int it = std::distance(dist.begin(),std::min_element(dist.begin(),dist.end()));
                if (dist[it] != 99999){
                    cpos = poi[it].coord;
                }
                else{
                    goto ch;
                }
              //getch();
                break;
            }
            case KEY_RIGHT:{
                std::vector<double> dist;
                for (int i=0; i<poi.size();i++){
                    if (poi[i].coord.x-cpos.x >= 2*std::abs(poi[i].coord.y-cpos.y) && // vertical
                        poi[i].coord.x-cpos.x != 0){ // it goes up
                            dist.push_back(distance(cpos, poi[i].coord));
                    }
                    else{
                        dist.push_back(99999);
                    }
                  //mvwprintw(stdscr, i, 0, "%d, %f",i,dist[i]);
                }
                int it = std::distance(dist.begin(),std::min_element(dist.begin(),dist.end()));
                if (dist[it] != 99999){
                    cpos = poi[it].coord;
                  //cpos = {poi[it].coord.y - anchor.y,poi[it].coord.x - anchor.x}; //test of anchor
                }
                else{
                    goto ch;
                }
              //getch();
                break;
            }

            case 'z':{
                switch (sc){
                    case 0:{
                        for (int i = 0; i<5; i++){
                            scale += 0.05;
                            drawCity(worldmap, city,scale);
                            if (scale >= 0.25){
                                drawShell(worldmap,dist,scale);
                            }
                            wrefresh(worldmap);
                            wait();
                        }
                        sc++;
                        break;
                    }
                    case 1:{
                        for (int i = 0; i<5; i++){
                            scale += 0.11;
                            drawCity(worldmap, city,scale);
                            if (scale >= 0.25){
                                drawShell(worldmap,dist,scale);
                            }
                            wrefresh(worldmap);
                            wait();
                        }
                        sc++;
                        break;
                    }
                    default:{
                        goto ch;
                    }
                }
                break;
            }
            case 'x':{
                switch (sc){
                    case 1:{
                        for (int i = 0; i<5; i++){
                            scale -= 0.05;
                            drawCity(worldmap, city,scale);
                            if (scale >= 0.25){
                                drawShell(worldmap,dist,scale);
                            }
                            wrefresh(worldmap);
                            wait();
                        }
                        sc--;
                        break;
                    }
                    case 2:{
                        for (int i = 0; i<5; i++){
                            scale -= 0.11;
                            drawCity(worldmap, city,scale);
                            if (scale >= 0.25){
                                drawShell(worldmap,dist,scale);
                            }
                            wrefresh(worldmap);
                            wait();
                        }
                        sc--;
                        break;
                    }
                    default:{
                        goto ch;
                    }
                }
                break;
            }
            default:{
                goto ch; //this skips the text rendering
            }
        }
    }
    //drawVector(city.points[0].y+0, city.points[0].x+0, city.points[1].y+0, city.points[1].x+0, 1, 0);
    //animVector();
}
