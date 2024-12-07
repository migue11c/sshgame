#include <cstdlib>
#include <functional>
#include <iostream>

int main() {
    char* in = (char*)std::malloc(16*sizeof(char));
    std::cin >> in;
    std::hash<char*> str_hash;
    std::cout << str_hash(in);
}
