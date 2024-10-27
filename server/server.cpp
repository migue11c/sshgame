#include <SFML/Audio/Listener.hpp>
#include <SFML/Config.hpp>
#include <SFML/Network.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/Socket.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include <fstream>
#include <iostream>
#include <string>

// MAKE SURE TO FIRST CHECK FOR HASHED KEY

int main(){

    sf::TcpListener listener;

    while (1){
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
        sf::Packet packet;
        if (client.receive(packet) != sf::Socket::Done){
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
                if (un != "miguell" || pw != "password"){ // replace this with actual un/pw checker
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
                plr:
                player.close();
                if (client.send(packet) != sf::Socket::Done){
                    goto plr;
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
