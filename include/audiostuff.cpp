#include <SFML/Audio.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/System/Sleep.hpp>
#include <thread>
#include "common.h"
#include "globals.h"

using namespace std::chrono_literals;

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
    bgm.setLoop(true);
    bgm.play();
    while (gameaudio::kill != true){
        std::this_thread::sleep_for(100ms);
    }
    return;
}

void loadMusic(){
    std::string dir = "resources/audio/" + gameaudio::musicdir;
    gameaudio::musicbuffer.loadFromFile(dir);
}

void dumpAudio(){
    sf::SoundBuffer clear;
    gameaudio::musicbuffer = clear;
    gameaudio::keybuffer = clear;
}
