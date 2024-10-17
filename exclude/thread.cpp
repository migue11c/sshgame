#include <thread>
#include <iostream>
#include "include/thread.h"


int main(){
    std::cout << "printig..." << std::endl;
    std::thread prin(spam);
    std::cout << "printed" << std::endl;
    prin.join();
}
