#ifndef _GLBL_H
#define _GLBL_H

#include <SFML/Audio.hpp>
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
}

struct playerData{
    std::string name;
    std::string faction;
    int maxhp;
    int hp;
    int dice[5];
    std::vector<int> items;
};

namespace player {
    inline playerData mydata;
    inline std::string username;
};

namespace gameaudio {
    inline sf::SoundBuffer keybuffer;
    inline sf::SoundBuffer musicbuffer;
}

#endif
