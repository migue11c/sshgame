#ifndef _GLBL_H
#define _GLBL_H

#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <string>
#include <vector>

struct vertex {
    double y,x;
};

struct timeAndDay {
    int day;
    int hour;
};

struct sector{ // this will essentially be player information
    int area,district,place; //area, district and place
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
    std::string faction; //needs to be uint and be interpreted by the client
    sector loc;
    int maxhp; //deprecated
    int hp; //deprecated
    timeAndDay time;
    int dice[5];
    bool items[100];
};

struct worldData {
    std::string worldName;
    unsigned int worldDay;
    unsigned int dangerLevel[25];
    itemData itemlist[100];
};

namespace world {
    inline std::string worldName;
    inline timeAndDay worldTime;
    inline int dangerLevel[26];
    inline itemData itemlist[100];
};

namespace player {
    inline playerData mydata;
    inline std::string username;
    inline timeAndDay playerTime;
};

namespace gameaudio {
    inline bool kill = false;
    const std::string musicdir = "menu.ogg";
    inline sf::SoundBuffer keybuffer[2];
    inline sf::Music music;
};

namespace clset {
    inline settings sets = {100,100};
};

#endif
