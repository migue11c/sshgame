#ifndef _WRLD_H
#define _WRLD_H

#include "globals.h"
#include <SFML/Network/Packet.hpp>

worldData readWD();
playerData readPD();
void writeWD(worldData in);
void writePD(playerData in);
sf::Packet packPlayer(playerData in);
playerData unpackPlayer(sf::Packet in);
sf::Packet packWorld(worldData in);
worldData unpackWorld(sf::Packet in);
void cityrender();

#endif
