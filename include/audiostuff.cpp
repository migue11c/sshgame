#include <SFML/Audio.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/System/Sleep.hpp>
#include <chrono>
#include "common.h"
#include "globals.h"


void loadAudio(){
    loadKey();
    loadMusic();
}

void playKey(){
    sf::Sound keybsound(gameaudio::keybuffer);
    keybsound.play();
    sf::sleep(gameaudio::keybuffer.getDuration());
    keybsound.resetBuffer();
    return;
}

void loadKey(){
    gameaudio::keybuffer.loadFromFile("kb1.wav");
}

void playMusic(){
    sf::Sound bgm(gameaudio::musicbuffer);
    bgm.play();
    sf::sleep(gameaudio::musicbuffer.getDuration());
    return;
}

void loadMusic(){
    gameaudio::musicbuffer.loadFromFile("menu.ogg");
}

void dumpAudio(){
    sf::SoundBuffer clear;
    gameaudio::musicbuffer = clear;
    gameaudio::keybuffer = clear;
}
