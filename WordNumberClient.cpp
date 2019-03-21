#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <fcntl.h>
#include <io.h>
// #include <locale>
// #include <codecvt>
// #include <memory>
#include "word_number.h"

#define SERVERADDRESS "127.0.0.1"
#define PORT 5555

void fail() {
    std::cout << "FAIL" << std::endl;
    system("pause");
}

int main() {
    _setmode(_fileno(stdout), _O_U16TEXT);
    // std::ios_base::sync_with_stdio(false);

    // typedef std::codecvt_utf8<wchar_t/*, 0x10ffff, std::consume_header*/> utf8cvt_t;

    // std::unique_ptr<utf8cvt_t> itf8Cvt(new utf8cvt_t);

    // std::locale ru(std::locale(), itf8Cvt.get());
    // std::wcout.imbue(ru);

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

        ushort numberSize = log10(abs((long double) number)) + 1;
        std::string sendBuffer = std::to_string(number);

        int sendSizeRet = send(serverConnection, (char*) &numberSize, sizeof(int), 0);
        int sendNumberRet = send(serverConnection, sendBuffer.c_str(), numberSize, 0);

        IF_CONNECTION_CLOSED(sendSizeRet)
        IF_CONNECTION_CLOSED(sendNumberRet)
        
        if (sendSizeRet == -1 || sendNumberRet == -1) {
            std::cout << "Client send failed" << std::endl;
            break;
        }

        ushort wordNumberLen;

        int lenRecvRet = recv(serverConnection, (char*) &wordNumberLen, sizeof(ushort), 0);

        wchar_t* receiveBuffer = new wchar_t[wordNumberLen + 1];

        receiveBuffer[wordNumberLen] = '\0';

        int wordNumRecvRet = recv(serverConnection, (char*) receiveBuffer, wordNumberLen, 0);

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