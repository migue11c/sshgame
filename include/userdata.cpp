#include <fstream>
#include <string>

#include "globals.h"
#include "player.h"
#include "logs.h"

void getSets(){
    std::string dir = "resources/settings";
    std::ifstream data(dir);
    if (data.bad()){
        ErrLog("ofstream: settingData bad location");
    }
    data >> clset::sets.BGMVolume >> clset::sets.SFXVolume;
    data.close();
}

void setSets(){
    std::string dir = "resources/settings";
    std::ofstream data(dir);
    if (data.bad()){
        ErrLog("ofstream: settingData bad location");
    }
    data << clset::sets.BGMVolume << " " << clset::sets.SFXVolume << "\n";
}

void getPlayerData(){
    std::string dir = "users/" + player::username + ".dat";
    std::ifstream data(dir);
    if (data.bad()){
        ErrLog("ifstream: bad player data location");
    }

    getline(data, player::mydata.name);
    getline(data, player::mydata.faction);
    data >> player::mydata.maxhp >> player::mydata.hp;

    int buffer; //adds data to vector
    for (int i = 0; i<5; i++){
        data >> player::mydata.dice[i];
    }
    int i = 0;
    while (!data.eof()){

        data >> buffer;
        player::mydata.items[i] = buffer;
        i++;
    }

    LogThis("retrieved %s.dat",player::username.c_str());
    data.close();
}

void writePlayerData(){
    std::string dir = "users\\" + player::username + ".dat";
    std::ofstream data(dir); //solve the binary issue for another day
    if (data.bad()){
        ErrLog("ofstream: bad player data location");
    }

    data << player::mydata.name << "\n" << player::mydata.faction << "\n" << player::mydata.maxhp << "\n" << player::mydata.hp << "\n";
    for (int i = 0; i < 5; i++){ // change this if you change amt of dice!!!
        data << player::mydata.dice[i] << " ";
    }
    data << "\n";
    for (int i = 0;i<100;i++){
        data << player::mydata.items[i] << " ";
    }
    LogThis("written %s.dat",player::mydata.name.c_str());
    data.close();
}

void addItem(itemData it){
    //please add a check to see if player has that id already
    player::mydata.items[it.id] = true;
    LogThis("added %s to inventory",it.name.c_str());
}

void playerCleanup(){

}
