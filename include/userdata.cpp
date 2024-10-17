#include <fstream>
#include <string>
#include <vector>

#include "player.h"
#include "logs.h"
using namespace std;

void getPlayerData(){
    string dir = "users/" + player::username + ".dat";
    ifstream data(dir);
    if (data.bad()){
        LogThis("bad data location");
    }

    getline(data, player::mydata.name);
    getline(data, player::mydata.faction);
    data >> player::mydata.maxhp;
    data >> player::mydata.hp;

    int buffer; //adds data to vector
    for (int i = 0; i<5; i++){
        data >> player::mydata.dice[i];
    }

    while (!data.eof()){
        data >> buffer;
        player::mydata.items.push_back(buffer);
    }

    LogThis("retrieved %s.dat",player::username.c_str());
    data.close();
}
void writePlayerData(){
    string dir = "users\\" + player::username + ".dat";
    ofstream data(dir); //solve the binary issue for another day
    data << player::mydata.name << "\n";
    data << player::mydata.faction << "\n";
    data << player::mydata.maxhp << "\n";
    data << player::mydata.hp << "\n";
    for (int i = 0; i < 5; i++){ // change this if you change amt of dice!!!
        data << player::mydata.dice[i] << " ";
    }
    data << "\n";
    for (int i = 0;i<player::mydata.items.size()-1;i++){
        data << player::mydata.items[i] << " ";
    }
    LogThis("written %s.dat",player::mydata.name.c_str());
    data.close();
}

void addItem(itemData it){
    //please add a check to see if player has that id already
    player::mydata.items.push_back(it.id);
    LogThis("added %s to inventory",it.name.c_str());
}

void playerCleanup(){

}
