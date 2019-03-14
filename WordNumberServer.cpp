#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>
// #include "word_number.h"
#include "oem866_word_number.h"

#define LOCALADDRESS "127.0.0.1"
#define PORT 5555

DWORD WINAPI serviceActiveClient(unsigned activeClientIdx);

SOCKET connections[100];
unsigned activeClientIdx = 0;

void printNumUsers() {
    if (activeClientIdx) {
        std::cout << activeClientIdx << " user online" << std::endl;
    } else {
        std::cout << "No user online" << std::endl;
    }
}

int main() {
    std::cout << "NUMTOSTR DEMO SERVER " << std::endl << std::endl;

    {
        WSADATA ws;
        if (WSAStartup(0x0202, &ws)) {
            std::cout << "WSA startup error " << WSAGetLastError() << std::endl;
            system("pause");
            return -1;
        }
    }

    sockaddr_in localAddress;
    int localAddrSize = sizeof(localAddress);
    localAddress.sin_family = AF_INET;
    localAddress.sin_addr.s_addr = inet_addr(LOCALADDRESS);
    localAddress.sin_port = htons(PORT);

    SOCKET sockListen = socket(AF_INET, SOCK_STREAM, 0);

    bind(sockListen, (SOCKADDR *) &localAddress, sizeof(localAddress));

    listen(sockListen, SOMAXCONN);

    std::cout << "Server started" << std::endl << std::endl;
    std::cout << "Waiting for connections" << std::endl << std::endl;

    SOCKET clientConnection;

    for (unsigned i = 0; i < 100; i++) {
        clientConnection = accept(sockListen, (SOCKADDR *) &localAddress, &localAddrSize);

        if (!clientConnection) {
            std::cout << "Connection error on " << activeClientIdx << " client" << std::endl << std::endl;
        }
        else {
            connections[i] = clientConnection;
            activeClientIdx++;
            
            CreateThread(0, 0, (LPTHREAD_START_ROUTINE) serviceActiveClient, (LPVOID) i, 0, 0);

            printNumUsers();
        }
    }

    WSACleanup();

    return 0;
}

// The function is created in a separate thread and serves
// the connected client, regardless of the rest
DWORD WINAPI serviceActiveClient(unsigned activeClientIdx) {

    #define IF_CONNECTION_CLOSED if (ret == 0) {\
                                    activeClientIdx--;\
                                    std::cout << " disconnected" << std::endl;\
                                    printNumUsers();\
                                    return -1;\
                                 }

    while (true)
    {
        // Receive the number and return the client a representation in words
        const unsigned long MAX_RECEIVE_BUFFER_LENGTH = 8;
        char receiveBuffer[MAX_RECEIVE_BUFFER_LENGTH];

        int ret = recv(connections[activeClientIdx], receiveBuffer, sizeof(receiveBuffer), 0);

        if (ret == -1) {
            std::cout << "Server receive failed" << std::endl;
        }

        IF_CONNECTION_CLOSED

        const ushort MAX_SEND_BUFFER_LENGTH = 60;
        char sendBuffer[MAX_SEND_BUFFER_LENGTH];

        unsigned number = atoi(receiveBuffer);

        if (number == 0) {
            strcpy(sendBuffer, "ноль");
        }

        if (number < 0) {
            strcat(sendBuffer, "минус ");
            number = -number;
        }

        const char* strNum = wnServer.format(number).c_str();

        strcat(sendBuffer, strNum);

        ret = send(connections[activeClientIdx], sendBuffer, sizeof(sendBuffer), 0);

        if (ret == -1) {
            std::cout << "Server send failed" << std::endl;
        }

        IF_CONNECTION_CLOSED
    }

    closesocket(connections[activeClientIdx]);
    
    return 0;
}