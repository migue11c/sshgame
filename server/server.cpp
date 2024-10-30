#include <SFML/Audio/Listener.hpp>
#include <SFML/Config.hpp>
#include <SFML/Network.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/Socket.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include <SFML/System/Time.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <csignal>

// MAKE SURE TO FIRST CHECK FOR HASHED KEY

// this func is copied over from login.cpp on the client
bool auth(std::string username, std::string password) {
    std::ifstream list("userlist");
    std::string buffer;
    while(list >> buffer){
        if (strcmp(username.c_str(), buffer.c_str())==0){
            list >> buffer;
            list.close();
            return (strcmp(password.c_str(), buffer.c_str())==0);
        }
    }
    list.close();
    return false;
}

int main(){

    int c=0;
    auto lam = [] (int i) { std::cout << "closing...\n"; exit(0); };

    //^C
    signal(SIGINT, lam);
    //abort()
    signal(SIGABRT, lam);
    //sent by "kill" command
    signal(SIGTERM, lam);
    //^Z
    //signal(SIGTSTP, lam);

    sf::TcpListener listener;

    while (c<50){
        //sf::Packet packet;
        start:
        if (listener.listen(53000) != sf::Socket::Done){
            goto start;
        }
        std::cout << "listen successful\n";

        sf::TcpSocket client;
        if (listener.accept(client) != sf::Socket::Done){
            goto start;
        }
        std::cout << client.getRemoteAddress() << " connected\n";
        // needs to terminate connection after client refuses to send a request

        sf::Packet packet;
        if (client.receive(packet) != sf::Socket::Done){
            client.disconnect();
            goto start;
        }
        std::string un,pw;
        sf::Uint8 check;
        sf::Uint32 key;
        packet >> check >> un >> pw; // replace this with key
        packet.clear();
        switch (check){
            case 0:{
                auth:
                if (!auth(un,pw)){ // username/password checker
                    packet << "neg0";
                    if (client.send(packet) != sf::Socket::Done){
                        goto auth;
                    }
                    std::cout << client.getRemoteAddress() <<" failed login auth (" << un << ", " << pw << ")\n";
                }
                else {
                    packet << "pos0";
                    if (client.send(packet) != sf::Socket::Done){
                        goto auth;
                    }
                    std::cout << client.getRemoteAddress() << " logged in as " << un << "\n";
                }
                break;
            }
            case 1:{
                std::string dir = "users/" + un + ".dat";
                std::string name,faction;
                std::ifstream player(dir);
                if (player.bad()){
                    break;
                }

                getline(player, name);
                getline(player, faction);
                packet << name << faction;
                sf::Uint64 buffer;
                char cbuf;
                bool num;
                for (int i = 0; i<5; i++){
                    player >> buffer;
                    packet << buffer;
                }
                for (int i = 0; i<100; i++){
                    player >> cbuf;
                    switch (cbuf) {
                        case 49:{
                            num = 1;
                            break;
                        }
                        case 48:{
                            num = 0;
                            break;
                        }
                        default:{
                            num = 0;
                            break;
                        }
                    }
                    packet << num;
                }
                player.close();
                plr:
                if (client.send(packet) != sf::Socket::Done){
                    goto plr;
                    //potentially infinite loop
                }
                std::cout << un << " received playerData\n";
                break;
            }
            case 2:{
                // this needs to be finished
                break;
            }
            default:{
                break;
            }
        }
    }
}
