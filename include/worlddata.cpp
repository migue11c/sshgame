#include <fstream>
#include <string>

#include "world.h"
#include "logs.h"

void getWorldData(){
    std::string dir = "world/" + world::worldName + ".dat";
    std::ifstream data(dir);
    if (data.bad()){
        LogThis("bad data location");
    }
    getline(data, world::worldName);
    data >> world::worldTime.day;
    data >> world::worldTime.hour;
    for (int i = 0; i<26; i++){
        data >> world::dangerLevel[i];
    }
    for (int i = 0; i<26; i++){
        getline(data, world::maplist[i].name);
        data >> world::maplist[i].x >> world::maplist[i].y >> world::maplist[i].id;
    }
    for (int i = 0; i<100; i++){
        getline(data, world::itemlist[i].name);
        getline(data, world::itemlist[i].effect);
        data >> world::itemlist[i].id;
    }
    LogThis("retrieved %s.dat",player::username.c_str());
}
void writeWorldData(){
    std::string dir = "world/" + player::username + ".dat";
    std::ofstream data(dir);
    data << world::worldName << std::endl;
    data << world::worldTime.day << std::endl;
    data << world::worldTime.hour << std::endl;
    for (int i = 0; i<26; i++){
        data << world::dangerLevel[i] << " ";
    }
    data << std::endl;
    for (int i = 0; i<52; i++){
        data << world::maplist[i].name << std::endl;
        data << world::maplist[i].x << " " << world::maplist[i].y << " " << world::maplist[i].id << " " << std::endl;
    }
    for (int i = 0; i<100; i++){
        data << world::itemlist[i].name << std::endl << world::itemlist[i].effect << world::itemlist[i].id << std::endl;
    }
}

void worldCleanup(){

}
