
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <cstdlib>
#include <iostream>

// client = 1, server = 0
const bool client = 1;
const int port = 12345;

int main()
{
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

        // socket connected, share some data
        char data[7] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
        if(socket.send(data, 7) != sf::Socket::Done)
        {
            std::cout << "Error sending data!\n";
            return 1;
        }

        std::size_t received;

        if(socket.receive(data, 7, received) != sf::Socket::Done)
        {
            std::cout << "Error receiving data!\n";
            return 1;
        }
        std::cout << "Received " << received << " bytes: " << data[0] << data[1] << data[2] << data[3] << data[4] << data[5] << data[6] << std::endl;
    }
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
        sf::TcpSocket client;
        if(listener.accept(client) != sf::Socket::Done)
        {
            std::cout << "Error accepting client!\n";
            return 1;
        }

        char data[7];
        std::size_t received;

        if(client.receive(data, 7, received) != sf::Socket::Done)
        {
            std::cout << "Error receiving data!\n";
            return 1;
        }
        std::cout << "Received " << received << " bytes: " << data[0] << data[1] << data[2] << data[3] << data[4] << data[5] << data[6] << std::endl;

        data[0] = data[6];
        data[6] = data[1];
        data[1] = data[5];
        data[5] = data[2];
        data[2] = data[4];
        data[4] = data[3];

        if(client.send(data, 7) != sf::Socket::Done)
        {
            std::cout << "Error sending data!\n";
            return 1;
        }
    }


    return 0;
}
