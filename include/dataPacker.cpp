#include <SFML/Network/Packet.hpp>

#include "globals.h"

// next 4 functions are server only
worldData readWD() {
    worldData out;
    return out;
}

playerData readPD() {
    playerData out;
    return out;
}

void writeWD(worldData in) {

}

void writePD(playerData in) {

}

// these are used by both server and client
sf::Packet packPlayer(playerData in) {
    sf::Packet out;
    return out;
}

playerData unpackPlayer(sf::Packet in) {
    playerData out;
    return out;
}

sf::Packet packWorld(worldData in) {
    sf::Packet out;
    return out;
}

worldData unpackWorld(sf::Packet in) {
    worldData out;
    return out;
}
