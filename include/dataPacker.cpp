#include <SFML/Config.hpp>
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
    out << in.name << in.faction << in.loc.area << in.loc.district
        << in.loc.place << in.maxhp << in.hp << in.time.day
        << in.time.hour;
        for (int i=0;i<6;i++) {
            out << in.dice[i];
        }
        for (int i=0;i<128;i++) {
            out << in.items[i];
        }
    return out;
}

playerData unpackPlayer(sf::Packet in) {
    playerData out;
    in >> out.name >> out.faction >> out.loc.area >> out.loc.district >> out.loc.place >> out.maxhp
        >> out.hp >> out.time.day >> out.time.hour;
        for (int i=0;i<6;i++){
            in >> out.dice[i];
        }
        for (int i=0;i<128;i++){
            in >> out.items[i];
        }
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
