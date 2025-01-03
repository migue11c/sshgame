#ifndef _GLBL_H
#define _GLBL_H

#include <string>
#include <vector>

struct vertex {
    double y,x;
};

struct timeAndDay {
    unsigned int day, hour;
};

struct sector{ // this will essentially be player information
    unsigned int area,district,place; //area, district and place
};

struct itemData {
    unsigned int id;
    unsigned int stack = 1;
    std::string name; //these need to be charrays
    std::string effect;
};

struct settings{
    int SFXVolume;
    int BGMVolume;
};

struct playerData{
    char name[16]; //needs to be char[16]
    unsigned int faction; //needs to be uint and be interpreted by the client
    sector loc;
    unsigned int maxhp; //deprecated
    unsigned int hp; //deprecated
    timeAndDay time;
    unsigned int dice[6];
    unsigned int items[128];
};

struct worldData {
    char motd[256];
    unsigned int worldDay;
    unsigned int dangerLevel[25];
    itemData itemlist[100];
};

namespace world {
    inline std::string worldName;
    inline timeAndDay worldTime;
    inline int dangerLevel[25];
    inline itemData itemlist[100];
};

namespace player {
    inline playerData mydata;
    inline std::string username;
};

namespace clset {
    inline settings sets = {100,100};
};

#endif
