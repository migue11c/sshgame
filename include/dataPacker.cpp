#include <SFML/Config.hpp>
#include <SFML/Network/Packet.hpp>
#include <fstream>

#include "globals.h"

// next 4 functions are server only
worldData readWD() {
    worldData out;
    return out;
}

playerData readPD(std::string un) { // needs testing
    std::string dir;
    dir = "users/" + un + ".dat";
    playerData out;
    std::ifstream data(dir);
    data >> out.name >> out.faction >> out.loc.area >> out.loc.district
        >> out.loc.place >> out.maxhp >> out.hp >> out.time.day
        >> out.time.hour;
    for (int i=0;i<6;i++) {
        data >> out.dice[i];
    }
    for (int i=0;i<128;i++) {
        data >> out.items[i];
    }

    return out;
}

void writeWD(worldData in) {

}

void writePD(playerData in, std::string un) { // needs testing
    std::string dir;
    dir = "users/" + un + ".dat";
    std::ofstream data(dir);
    data << in.name << " " << in.faction << " " << in.loc.area
        << " " << in.loc.district << " " << in.loc.place << " "
        << in.maxhp << " " << in.hp << " " << in.time.day << " "
        << in.time.hour << "\n";
    for (int i=0;i<6;i++) {
        data << in.dice[i];
    }
    for (int i=0;i<128;i++) {
        data << in.items[i];
    }
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
