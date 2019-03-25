#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include "word_number.h"

#define SERVERADDRESS "127.0.0.1"
#define PORT 5555

void fail() {
    std::cout << "FAIL" << std::endl;
    system("pause");
}

int main() {
    setlocale(LC_ALL, "");

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

    #define IF_CONNECTION_CLOSED(ret) if (ret == 0) { \
                                          std::cout << "Connection closed" << std::endl; \
                                          return -1; \
                                      }

    while (std::cin.peek() != 'q') {
        int number;

        std::cin >> number;

        int sendNumberRet = send(serverConnection, (char*) &number, sizeof(int), 0);

        IF_CONNECTION_CLOSED(sendNumberRet)
        
        if (sendNumberRet == -1) {
            std::cout << "Client send failed" << std::endl;
            break;
        }

        ushort wordNumberLen;

        int lenRecvRet = recv(serverConnection, (char*) &wordNumberLen, sizeof(ushort), 0);

        wchar_t* receiveBuffer = new wchar_t[wordNumberLen + 1];

        receiveBuffer[wordNumberLen] = '\0';

        int wordNumRecvRet = recv(serverConnection, (char*) receiveBuffer, wordNumberLen + wordNumberLen, 0);

        IF_CONNECTION_CLOSED(lenRecvRet)
        IF_CONNECTION_CLOSED(wordNumRecvRet)
        
        if (lenRecvRet == -1 || wordNumRecvRet == -1) {
            std::cout << "Client receive failed" << std::endl;
            break;
        }
   
        std::wcout << receiveBuffer << std::endl;

        delete[] receiveBuffer;

        std::cin.ignore();
    }

    closesocket(serverConnection);

    WSACleanup();

    return 0;
}