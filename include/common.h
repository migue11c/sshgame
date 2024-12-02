#ifndef _CMN_H
#define _CMN_H

#include <ncurses/ncurses.h>
#include <string>
#include "globals.h"

void textAnimation(WINDOW* win, std::string text, int len, int y, int x, int offs);
void drawLogo(int ymax, int xmax);
void settingsScreen(int ymax, int xmax);
int mainScreenOptions(int ymax, int xmax);
void wait();
void drawVector(WINDOW* win, int sty, int stx, int finy, int finx, int div, int offs);
void cityrender();

#endif
