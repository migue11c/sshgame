#ifndef _RNDR_H
#define _RNDR_H

#include <ncurses/ncurses.h>
#include <vector>
#include "globals.h"

struct poi {
    vertex coord;
    unsigned int cond;
    char name[24];
    unsigned int len;
    float scl;
};
inline vertex cpos{0,0}; //THIS IS TEMPORARY AND NEEDS TO BE INITIALIZED BY SECTOR
inline vertex anchor{96,235}; //this needs to be determined by a function

struct shell {
    vertex points[70];
};
struct map {
    vertex points[30];
};
struct wall {
    std::vector<vertex> points;
};

shell getShell();
void drawShell(WINDOW* win, shell dist, double scale);
map getMap();
void drawCity(WINDOW* win, map city, double scale);
void drawPos(vertex cam, double scale);
double distance(vertex st, vertex en);
std::vector<poi> getPoi();
void drawPoi(WINDOW* win, std::vector<poi> poi, double scale, int offs);

#endif
