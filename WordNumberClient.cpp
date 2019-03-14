#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
// #include "word_number.h"
#include "oem866_word_number.h"

#define SERVERADDRESS "127.0.0.1"
#define PORT 5555

void fail() {
    std::cout << "FAIL" << std::endl;
    system("pause");
}

int main() {

    std::cout << "NUMTOSTR DEMO CLIENT" << std::endl;

    {
        WSADATA ws;
        if (WSAStartup(0x0202, &ws)) {
            std::cout << "WSA startup error " << WSAGetLastError() << std::endl;
            system("pause");
            return -1;
        }
    }

    // Specify the server address and port
    sockaddr_in serverAddress;
    int serverAddrSize = sizeof(serverAddress);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(SERVERADDRESS);
    serverAddress.sin_port = htons(PORT);

    SOCKET serverConnection = socket(AF_INET, SOCK_STREAM, 0);

    // Establish the connection
    if (connect(serverConnection, (SOCKADDR *) &serverAddress, serverAddrSize)) {
        fail();
        std::cout << "Failed connect to server." << std::endl;
        return -1;
    }

    #define IF_CONNECTION_CLOSED if (!ret) {  \
                                    std::cout << "Connection closed" << std::endl; \
                                    return -1;  \
                                 }

    while (std::cin.peek() != 'q') {
        const ushort MAX_SEND_BUF_SIZE = 8;
        char sendBuffer[MAX_SEND_BUF_SIZE];

        unsigned number;

        std::cin >> number;

        sprintf(sendBuffer, "%d", number);

        int ret = send(serverConnection, sendBuffer, sizeof(sendBuffer), 0);
        
        if ( ret == -1 ) {
            std::cout << "client send failed" << std::endl;
        }

        IF_CONNECTION_CLOSED

        const ushort MAX_RECEIVE_BUFFER_LENGTH = 60;
        char receiveBuffer[MAX_RECEIVE_BUFFER_LENGTH];

        ret = recv(serverConnection, receiveBuffer, sizeof(receiveBuffer), 0);
        
        if (ret == -1) {
            std::cout << "client receive failed" << std::endl;
        }

        IF_CONNECTION_CLOSED
   
        std::cout << receiveBuffer << std::endl;

        std::cin.ignore();
    }

    closesocket(serverConnection);

    WSACleanup();

    return 0;
}