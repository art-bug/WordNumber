#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <thread>
#include "word_number.h"

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

            std::thread thread(serviceActiveClient, i);

            printNumUsers();

            thread.join();
        }
    }

    WSACleanup();

    return 0;
}

// The function is created in a separate thread and serves
// the connected client, regardless of the rest
DWORD WINAPI serviceActiveClient(unsigned activeClientIdx) {

    #define IF_CONNECTION_CLOSED(ret) if (ret == 0) { \
                                          std::cout << activeClientIdx << " user disconnected" << std::endl; \
                                          activeClientIdx--; \
                                          printNumUsers(); \
                                          return -1; \
                                      }

    while (true)
    {
        int number;

        int numberRecvRet = recv(connections[activeClientIdx], (char*) &number, sizeof(int), 0);

        IF_CONNECTION_CLOSED(numberRecvRet)

        if (numberRecvRet == -1) {
            std::cout << "Server receive failed" << std::endl;
            break;
        }

        std::wstring sendBuffer = wnServer.format(number);

        ushort wordNumSize = sendBuffer.size();

        int sendStrSizeRet = send(connections[activeClientIdx], (char*) &wordNumSize, sizeof(ushort), 0);

        int wordNumSendRet = send(connections[activeClientIdx], (const char*) sendBuffer.c_str(), wordNumSize + wordNumSize, 0);

        IF_CONNECTION_CLOSED(sendStrSizeRet)
        IF_CONNECTION_CLOSED(wordNumSendRet)

        if (sendStrSizeRet == -1 || wordNumSendRet == -1) {
            std::cout << "Server send failed" << std::endl;
            break;
        }
    }

    closesocket(connections[activeClientIdx]);
    
    return 0;
}