#include <fstream>
#include <string>

std::ofstream logs("log.txt");

void LogThis(std::string txt,...){
    logs << txt << std::endl;
}

void ErrLog(std::string txt,...){
    logs << "ERROR: " << txt << std::endl;
}
