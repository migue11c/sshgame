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
    int id;
    int stack = 1;
    std::string name;
    std::string effect;
};

struct settings{
    int SFXVolume;
    int BGMVolume;
};

struct playerData{
    std::string name;
    std::string faction;
    int maxhp;
    int hp;
    int dice[5];
    bool items[100];
    sector loc;
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
