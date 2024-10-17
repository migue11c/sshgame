#include <SFML/Audio.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <iostream>

int main(){
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("rahh.mp3"))
        return 1;
    std::cout << "rahh.mp3:" << std::endl;
    std::cout << " " << buffer.getDuration().asSeconds() << " seconds" << std::endl;
    std::cout << " " << buffer.getSampleRate() << " samples / sec" << std::endl;
    std::cout << " " << buffer.getChannelCount() << " channels" << std::endl;
    sf::Sound sound(buffer);
    sound.play();

    while (sound.getStatus() == sf::Sound::Playing){
        sf::sleep(sf::milliseconds(25));
        std::cout << "\rPlaying... " << sound.getPlayingOffset().asMilliseconds() << " sec   ";
        std::cout << std::flush;
    }
    std::cout << std::endl << std::endl;
    return 0;
}
