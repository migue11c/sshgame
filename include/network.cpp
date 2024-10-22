#include <SFML/Config.hpp>
#include <SFML/Network.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/Socket.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <ncurses/ncurses.h>
#include "globals.h"
#include "logs.h"

// philosophy behind this client-server interaction is that the client ALWAYS makes requests
// and expects a result. If the result is not sent over, a new connection is attempted.
// server SHOULD NOT care about receiving a proper response and thread shuts down as soon
// as the packets are send, or failed to send.

// 0: logged in
// 1: failed login
// 2: timed out
// 3: sending playerData

int fetchData(std::string un, std::string pw){
    int value;
    int att = 0;
    sf::Uint8 i = 0;

    std::string conf;
    sf::TcpSocket socket;
    sf::Time timeout = sf::seconds(5);
    sf::Clock clock;
    sf::Packet packet;

    start:
    if(clock.getElapsedTime() == sf::seconds(20)){
        value = 2;
        return value;
    }
    sf::Socket::Status status = socket.connect("miguell.duckdns.org", 23);

    if (att <= 20){
        // code goes here
        if (status != sf::Socket::Done){
            goto back;
        }
        LogThis("Connected to server");
        packet << i << un << pw; // i will be parsed by the server switch statement

        // please also add a hashed password to the initial packet
        switch (i) {
            case 0: { // login
                if (socket.send(packet) != sf::Socket::Done){ // failed to send credentials
                    goto back;
                }
                if (socket.receive(packet) != sf::Socket::Done){ // failed to receive packet
                    goto back;
                }

                packet >> conf; // gets confirmation
                if (conf != "pos0"){ // failed to authenticate
                    value = 1;
                    break;
                }
                player::username = un;
                packet.clear();
                LogThis("Auth passed...");
                i++;

                goto back;
            }
            case 1: { // playerdata
                if (socket.send(packet) != sf::Socket::Done){ // failed to send credentials
                    goto back;
                }
                if (socket.receive(packet) != sf::Socket::Done){ // failed to receive packet
                    goto back;
                }

                packet >> player::mydata.name >> player::mydata.faction >> player::playerTime.hour >> player::mydata.maxhp >> player::mydata.hp; // gets confirmation
                for (int l = 0; l < 5; l++){
                    packet >> player::mydata.dice[l];
                }
                for (int l = 0; l < 100; l++){
                    packet >> player::mydata.items[l];
                }

                packet.clear();
                LogThis("playerData fetched...");
                i++;

                goto back;
            }
            case 2: { // worlddata
                if (socket.send(packet) != sf::Socket::Done){ // failed to send credentials
                    goto back;
                }
                if (socket.receive(packet) != sf::Socket::Done){ // failed to receive packet
                    goto back;
                }

                packet >> world::worldName >> world::worldTime.day;
                for (int l = 0; l < 26; l++){
                    packet >> world::dangerLevel[l];
                }

                // do not include maplist and itemlist, or worldtime hour

                packet.clear();
                LogThis("playerData fetched...");

                break;
            }
            default: {
                ErrLog("invalid fetch counter");
                break;
            }
        }
    }


    return value;

    back:
    sf::sleep(sf::seconds(0.5));
    clock.restart();
    att++;
    goto start;
}
