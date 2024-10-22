#ifndef _GLBL_H
#define _GLBL_H

#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <string>
#include <vector>

struct yxpoint {
    int y;
    int x;
};

struct timeAndDay {
    int day;
    int hour;
};

struct itemData {
    int id;
    int stack = 1;
    std::string name;
    std::string effect;
};

struct mapDetails {
    int x;
    int y;
    int id;
    std::string name;
};

namespace world {
    inline std::string worldName;
    inline timeAndDay worldTime;
    inline int dangerLevel[26];
    inline mapDetails maplist[52];
    inline itemData itemlist[100];
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
};

namespace player {
    inline playerData mydata;
    inline std::string username;
    inline timeAndDay playerTime;
};

namespace gameaudio {
    inline bool kill = false;
    const std::string musicdir = "music/menu.ogg";
    inline sf::SoundBuffer keybuffer[2];
    inline sf::Music music;
};

namespace clset {
    inline settings sets = {100,100};
};

#endif
