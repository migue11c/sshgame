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
