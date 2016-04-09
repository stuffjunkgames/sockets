
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <cstdlib>
#include <iostream>
#include <time.h>

// client = 1, server = 0
const bool client = 1;
const int port = 12345;

int main()
{
    // client
    if(client)
    {
        std::cout << "Creating socket on port " << port << "...\n";
        sf::TcpSocket socket;
        sf::Socket::Status status = socket.connect("msquared169.ddns.net", port);
        std::cout << "Connected socket\n";
        if(status != sf::Socket::Done)
        {
            std::cout << "Error connecting socket!\n";
            return 1;
        }

        sf::Uint16 x;
        std::string s;
        sf::Packet packet;
        if(socket.receive(packet) != sf::Socket::Done)
        {
            std::cout << "Error receiving data!\n";
            return 1;
        }

        packet >> x;

        if(x == 1)
        {
            std::cout << "Server selected you to go first.  Type something:\n";

            while(1)
            {
                std::cin >> s;

                packet << s;
                if(socket.send(packet) != sf::Socket::Done)
                {
                    std::cout << "Error sending data!\n";
                    return 1;
                }

                if(s == "exit")
                {
                    break;
                }

                if(socket.receive(packet) != sf::Socket::Done)
                {
                    std::cout << "Error receiving data!\n";
                    return 1;
                }

                packet >> s;
                std::cout << s << std::endl;

                if(s == "exit")
                {
                    break;
                }
            }

        }
        else if(x == 0)
        {
            std::cout << "Server selected other client to go first.  Please wait.";

            while(1)
            {
                if(socket.receive(packet) != sf::Socket::Done)
                {
                    std::cout << "Error receiving data!\n";
                    return 1;
                }

                packet >> s;
                std::cout << s << std::endl;

                if(s == "exit")
                {
                    break;
                }

                std::cin >> s;

                packet << s;
                if(socket.send(packet) != sf::Socket::Done)
                {
                    std::cout << "Error sending data!\n";
                    return 1;
                }

                if(s == "exit")
                {
                    break;
                }
            }
        }
        else
        {
            std::cout << "Received something other than 0 or 1\n";
        }
    }
    // server
    else
    {
        sf::TcpListener listener;

        // bind listener to port
        if(listener.listen(port) != sf::Socket::Done)
        {
            std::cout << "Error listening to port!\n";
            return 1;
        }

        // accept a new connection
        sf::TcpSocket client1;
        sf::TcpSocket client2;
        std::cout << "Listening for client connections...\n";
        if(listener.accept(client1) != sf::Socket::Done || listener.accept(client2) != sf::Socket::Done)
        {
            std::cout << "Error accepting client!\n";
            return 1;
        }

        std::cout << "Both clients connected!\n";

        // pick a random client to go first
        std::srand(std::time(NULL));
        int number = rand() % 2;
        sf::Uint16 x;
        sf::Packet packet;
        if(number == 0)
        {
            std::cout << "Chose client 1 to go first\n";
            x = 1;
            packet << x;
            if(client1.send(packet) != sf::Socket::Done)
            {
                std::cout << "Error sending packet to client 1\n";
                return 1;
            }

            x = 0;
            packet << x;
            if(client2.send(packet) != sf::Socket::Done)
            {
                std::cout << "Error sending packet to client 2\n";
                return 1;
            }

            std::string s;
            while(1)
            {
                if(client1.receive(packet) != sf::Socket::Done)
                {
                    std::cout << "Error receiving packet from client 1\n";
                    return 1;
                }

                if(client2.send(packet) != sf::Socket::Done)
                {
                    std::cout << "Error sending packet to client 2\n";
                    return 1;
                }

                packet >> s;
                std::cout << s << std::endl;
                if(s == "exit")
                {
                    break;
                }

                if(client2.receive(packet) != sf::Socket::Done)
                {
                    std::cout << "Error receiving packet from client 2\n";
                    return 1;
                }

                if(client1.send(packet) != sf::Socket::Done)
                {
                    std::cout << "Error sending packet to client 1\n";
                    return 1;
                }

                packet >> s;
                std::cout << s << std::endl;
                if(s == "exit")
                {
                    break;
                }
            }
        }
        else if(number == 1)
        {
            std::cout << "Chose client 2 to go first\n";
            x = 1;
            packet << x;
            if(client2.send(packet) != sf::Socket::Done)
            {
                std::cout << "Error sending packet to client 2\n";
            }

            x = 0;
            packet << x;
            if(client1.send(packet) != sf::Socket::Done)
            {
                std::cout << "Error sending packet to client 1\n";
            }

            std::string s;
            while(1)
            {
                if(client2.receive(packet) != sf::Socket::Done)
                {
                    std::cout << "Error receiving packet from client 2\n";
                    return 1;
                }

                if(client1.send(packet) != sf::Socket::Done)
                {
                    std::cout << "Error sending packet to client 1\n";
                    return 1;
                }

                packet >> s;
                std::cout << s << std::endl;
                if(s == "exit")
                {
                    break;
                }

                if(client1.receive(packet) != sf::Socket::Done)
                {
                    std::cout << "Error receiving packet from client 1\n";
                    return 1;
                }

                if(client2.send(packet) != sf::Socket::Done)
                {
                    std::cout << "Error sending packet to client 2\n";
                    return 1;
                }

                packet >> s;
                std::cout << s << std::endl;
                if(s == "exit")
                {
                    break;
                }
            }
        }

    }


    return 0;
}
