#include "../include/player.h"
#include "../include/logs.h"
#include <ncurses/ncurses.h>


int main(){
    initscr();
    player::username = "testname";
    getPlayerData();
    player::username = "testname";
    LogThis(player::mydata.name);
    LogThis(player::mydata.faction);
//  player::mydata.name = "pmijacic";
//  player::mydata.faction = "backst_rat";
//  player::mydata.maxhp = 10;
//  player::mydata.hp = 5;
//  player::mydata.dice[0] = 1;
//  player::mydata.dice[1] = 1;
//  player::mydata.dice[2] = 5;
//  player::mydata.dice[3] = 1;
//  player::mydata.dice[4] = 2;
//  player::mydata.items.push_back(5);
//  player::mydata.items.push_back(2);
//  player::mydata.items.push_back(4);
//  player::mydata.items.push_back(7);


    writePlayerData();
    drawUser();
    getch();
    endwin();
}
