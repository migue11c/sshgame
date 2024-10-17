#include <SFML/Audio.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include "common.h"

void playKey(){
    sf::SoundBuffer buffer;
    buffer.loadFromFile("kb1.wav");
    sf::Sound key(buffer);
    key.play();
    while (key.getStatus() == sf::Sound::Playing){}
    key.resetBuffer();
    return;
}

void loadKey(){
}
