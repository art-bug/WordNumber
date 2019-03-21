#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
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

    #define IF_CONNECTION_CLOSED(ret) if (ret == 0) { \
                                          activeClientIdx--; \
                                          std::cout << " disconnected" << std::endl; \
                                          printNumUsers(); \
                                          return -1; \
                                      }

    while (true)
    {
        // Receive the number and return the client a representation in words
        ushort numSize;

        int numSizeRecvRet = recv(connections[activeClientIdx], (char*) &numSize, sizeof(ushort), 0);

        char* receiveBuffer = new char[numSize + 1];

        receiveBuffer[numSize] = '\0';

        int numRecvRet = recv(connections[activeClientIdx], receiveBuffer, numSize, 0);

        IF_CONNECTION_CLOSED(numSizeRecvRet)
        IF_CONNECTION_CLOSED(numRecvRet)

        if (numSizeRecvRet == -1 || numRecvRet == -1) {
            std::cout << "Server receive failed" << std::endl;
            break;
        }

        int number = atoi(receiveBuffer);

        delete[] receiveBuffer;

        std::wstring sendBuffer;

        if (number < 0) {
            sendBuffer = L"минус ";
            number = -number;
        }

        if (number == 0) {
            sendBuffer = L"ноль";
        }
        else {
            sendBuffer = wnServer.format(number);
        }

        ushort wordNumSize = sendBuffer.size();

        int sendStrSizeRet = send(connections[activeClientIdx], (char*) &wordNumSize, sizeof(ushort), 0);

        int wordNumSendRet = send(connections[activeClientIdx], (char*) sendBuffer.c_str(), wordNumSize, 0);

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