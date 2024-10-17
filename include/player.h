#ifndef _PLYR_H
#define _PLYR_H

#include <string>
#include "globals.h"

bool login(int ymax, int xmax);
bool auth(std::string username, std::string password); //this relies on a different username value
//data
void getPlayerData();
void writePlayerData();
void addItem(itemData it);
void playerCleanup();
void drawUser();

#endif
