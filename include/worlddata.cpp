#include <fstream>
#include <string>

#include "world.h"
#include "logs.h"
using namespace std;

void getWorldData(){
    string dir = "world/" + world::worldName + ".dat";
    ifstream data(dir);
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
    string dir = "world/" + player::username + ".dat";
    ofstream data(dir);
    data << world::worldName << endl;
    data << world::worldTime.day << endl;
    data << world::worldTime.hour << endl;
    for (int i = 0; i<26; i++){
        data << world::dangerLevel[i] << " ";
    }
    data << endl;
    for (int i = 0; i<52; i++){
        data << world::maplist[i].name << endl;
        data << world::maplist[i].x << " " << world::maplist[i].y << " " << world::maplist[i].id << " " << endl;
    }
    for (int i = 0; i<100; i++){
        data << world::itemlist[i].name << endl << world::itemlist[i].effect << world::itemlist[i].id << endl;
    }
}

void worldCleanup(){

}
