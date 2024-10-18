#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/System/Sleep.hpp>
#include <thread>
#include "common.h"
#include "globals.h"

using namespace std::chrono_literals;

void loadAudio(){
    loadKey();
    //loadMusic();
}

void playKey(){
    sf::Sound keybsound(gameaudio::keybuffer);
    keybsound.setVolume(30.0f);
    keybsound.play();
    sf::sleep(gameaudio::keybuffer.getDuration());
    keybsound.resetBuffer();
    return;
}

void loadKey(){
    gameaudio::keybuffer.loadFromFile("kb1.mp3");
}

void playMusic(){
    sf::Music pla;
    std::string dir = "resources/audio/" + gameaudio::musicdir;
    pla.openFromFile(dir);
    pla.setLoop(true);
    pla.play();
    while (gameaudio::kill != true){
        std::this_thread::sleep_for(100ms);
    }
    return;
}

void loadMusic(){
}

void dumpAudio(){
    sf::SoundBuffer clear;
    sf::Music clearm;
    gameaudio::keybuffer = clear;
}
