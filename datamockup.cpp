#include "include//data.h"
#include "include//globals.h"
#include <SFML/Network/Packet.hpp>
#include <cstring>
#include <algorithm>
#include <iterator>
#include <iostream>
int main(){
    playerData mydata;
    strcpy(mydata.name, "michael");
    mydata.faction = 2;
    mydata.loc = {1,4,6};
    mydata.maxhp = 50;
    mydata.hp = 15;
    mydata.time = {1,5};

    mydata.dice[0] = 6;
    mydata.dice[1] = 2;
    mydata.dice[2] = 3;
    mydata.dice[3] = 2;
    mydata.dice[4] = 6;
    mydata.dice[5] = 7;

    unsigned int newarr[] = {1,1,0,1,0,1,0,0,0,1,0};
    std::copy(std::begin(newarr),std::end(newarr),std::begin(mydata.items));

    sf::Packet temp = packPlayer(mydata);
    playerData out = unpackPlayer(temp);
    std::cout << "\n" << out.name << "\n" << out.faction << "\n" << out.loc.area << "\n"
        << out.loc.district << "\n" << out.loc.place << "\n" << out.maxhp << "\n"
        << out.hp << "\n" << out.time.day << "\n" << out.time.hour << "\n" << mydata.dice[4]
        << "\n" << mydata.items[6];
}
