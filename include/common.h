#ifndef _CMN_H
#define _CMN_H

#include <string>
#include "globals.h"

void textAnimation(std::string text, int y, int x);
void loadKey();
void playKey();
void drawLogo(int ymax, int xmax);
void settingsScreen(int ymax, int xmax);
int mainScreenOptions(int ymax, int xmax);
void wait();
void drawVector(int sty, int stx, int finy, int finx, int div, int offs);
void animVector();

#endif
