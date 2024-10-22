#include <SFML/Network.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/Socket.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include "globals.h"
#include "logs.h"

// 0: logged in
// 1: failed login
// 2: connection error
// 3: sending playerData

int fetchData(std::string un, std::string pw){
    int value;
    int att = 0;
    int i = 0;

    std::string conf;
    sf::TcpSocket socket;
    sf::Clock clock;
    sf::Packet packet;

    start:
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

                if (conf != "pos0"){ // failed to authenticate
                    value = 1;
                    break;
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

                if (conf != "pos0"){ // failed to authenticate
                    value = 1;
                    break;
                }
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
