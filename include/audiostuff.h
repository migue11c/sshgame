#ifndef _AUD_H
#define _AUD_H

#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include "globals.h"

namespace gameaudio {
    inline bool kill = false;
    const std::string musicdir = "menu.ogg";
    inline sf::SoundBuffer keybuffer[2];
    inline sf::Music music;
};

void playMusic();
void loadKey();
void playKey(const int &i);
void loadAudio();
void dumpAudio();

#endif
