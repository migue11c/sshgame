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
        sf::Packet packet;
        if (listener.listen(53000) != sf::Socket::Done){
            // listener error
        }
        std::cout << "listen successful\n";
        sf::TcpSocket client;
        if (listener.accept(client) != sf::Socket::Done){
            // accept error
        }
        std::cout << "client accepted\n";
        if (client.receive(packet) != sf::Socket::Done){
            // packet reception error
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
            std::cout << "bad login\n";
        }
        else {
            packet << "pos0";
            if (client.send(packet) != sf::Socket::Done){
                goto auth;
            }
            std::cout << "correct login\n";
        }

    }

    return 0;
}
