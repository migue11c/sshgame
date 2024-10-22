#include <SFML/Audio/Listener.hpp>
#include <SFML/Config.hpp>
#include <SFML/Network.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/Socket.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include <iostream>
#include <string>

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
        packet >> check >> un >> pw;
        packet.clear();
        auth:
        if (un != "miguell" || pw != "thskwaeiw"){
            packet << "neg0";
            if (client.send(packet) != sf::Socket::Done){
                goto auth;
            }
            std::cout << client.getRemoteAddress() <<" failed login auth\n";
        }
        else {
            packet << "pos0";
            if (client.send(packet) != sf::Socket::Done){
                goto auth;
            }
            std::cout << client.getRemoteAddress() << " logged in as " << "miguell" << "\n";
        }
    }
}
