#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>
// Winsock2.h must be connected before windows.h
#include <windows.h>
#include <locale.h>
#include "word_number.h"

#define PORT 5555

#define PRESS_ANY_KEY std::cin.ignore(); std::cin.get();

DWORD WINAPI serviceActiveClient(LPVOID clientSocket);

int numActiveClients = 0;

void printNumUsers() {
    if (numActiveClients) {
        std::cout << numActiveClients << " user online" << std::endl;
    } else {
        std::cout << "No user online" << std::endl;
    }
}

int main() {
    setlocale(LC_ALL, "RUS");

    std::cout << "NUMTOSTR DEMO SERVER " << std::endl << std::endl;

    // Initialize the Winsock library
	// Since the information returned by the function is not used,
	// it is passed a pointer to the working buffer, converted to
	// a pointer to the WSADATA structure. This method allows you to manage
	// one variable, however, the buffer must be at least
	// half a kilobyte (WSADATA structure occupies 400 bytes)

    if (WSAStartup(0x0202, new WSADATA[400])) {
        std::cout << "WSA startup error " << WSAGetLastError() << std::endl;
        PRESS_ANY_KEY
        return -1;
    }

	initRankChain();

    // Create a socket
	// AF_INET - Internet socket
	// SOCK_STREAM - stream socket (with connection setup)
	// TCP protocol is automatically selected

    SOCKET sock = 0;
    if ( !(sock = socket(AF_INET, SOCK_STREAM, 0))) {
        std::cout << "Socket error " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }

    sockaddr_in localAddress = {};
    localAddress.sin_family = AF_INET;
    localAddress.sin_port = htons(PORT);
    localAddress.sin_addr.s_addr = 0;
    // The server accepts connections to all IP addresses

    if ( bind(sock, (sockaddr *) &localAddress, sizeof(localAddress)) ) {
        std::cout << "Bind error " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    // Waiting for connections
    // Queue size = 5
    if (listen(sock, 5)) {
        std::cout << "Listen error " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    std::cout << "Server started" << std::endl << std::endl;
    std::cout << "Waiting for connections" << std::endl << std::endl;

    SOCKET clientSocket = 0;
    sockaddr_in clientAddress = {};

    int clientAddressSize = sizeof(clientAddress);

    // Loop request queue
    while ( (clientSocket = accept(sock, (sockaddr *)
                            &clientAddress, &clientAddressSize)) )
    {
        // Call a new thread to serve the client
        // For this we recommend using _beginthreadex,
        // but, since there are no calls to the standard C functions of the library
        // the thread does not, you can do CreateThread
        DWORD threadID = 0;
        CreateThread(0, 0, serviceActiveClient, &clientSocket, 0, &threadID);

        numActiveClients++;

        HOSTENT *pHost = gethostbyaddr((char *) &clientAddress.sin_addr.s_addr, 4, AF_INET);

        printNumUsers();
    }

    WSACleanup();

    return 0;
}

// The function is created in a separate thread and serves
// the connected client, regardless of the rest
DWORD WINAPI serviceActiveClient(LPVOID clientSocket) {
    SOCKET socket = ((SOCKET *) clientSocket)[0];

    // Receive the number and return the client a representation in words
    const unsigned long MAX_RECEIVE_BUFFER_LENGTH = 8;
    char receiveBuffer[MAX_RECEIVE_BUFFER_LENGTH];
    char* receiveBufCurrentPos = receiveBuffer;

    #define IF_CONNECTION_CLOSED if ( ret == 0 ) {\
                                    numActiveClients--;\
                                    std::cout << " disconnected" << std::endl;\
                                    printNumUsers();\
                                    return -1;\
                                 }

    while( (unsigned long)(receiveBufCurrentPos - receiveBuffer) < MAX_RECEIVE_BUFFER_LENGTH )
    {
        int ret = recv(socket, (char *) receiveBufCurrentPos, 
                               int(MAX_RECEIVE_BUFFER_LENGTH - (receiveBufCurrentPos - receiveBuffer)), 0);
        if ( ret == -1 ) {
            std::cout << "Server receive failed" << std::endl;
            receiveBufCurrentPos = (char *) &ret;
        }

        IF_CONNECTION_CLOSED

        receiveBufCurrentPos += ret;

        if ( (unsigned long)(receiveBufCurrentPos - receiveBuffer) < MAX_RECEIVE_BUFFER_LENGTH ) 
        {
            std::cout << "[recv notice] attempt to receive " << MAX_RECEIVE_BUFFER_LENGTH << " bytes but " 
                      << (unsigned long)(receiveBufCurrentPos - receiveBuffer) << " received per call ("
                      << (MAX_RECEIVE_BUFFER_LENGTH - ((unsigned long)(receiveBufCurrentPos - receiveBuffer)))
                      << " left to receive)" << std::endl;
        }
    }

    const unsigned long MAX_SEND_BUFFER_LENGTH = 1024;
    char sendBuffer[MAX_SEND_BUFFER_LENGTH] = {0};

    unsigned long number = atoi(receiveBuffer);

    number = ntohl(number);

    if (number == 0) {
        strcpy(sendBuffer, "ноль");
    }

    if (number < 0) {
        strcat(sendBuffer, "минус ");
        number = -number;
    }

    int numSize = int(log10(abs( (long double)number)) + 1);

    const char* strNum = rankChain[numSize]->format(number);

    strcat(sendBuffer, strNum);

    const char* sendBufCurrentPos = sendBuffer;

    while( (unsigned long)(sendBufCurrentPos - sendBuffer) < MAX_SEND_BUFFER_LENGTH )
    {
        int ret = send(socket, (const char *) sendBufCurrentPos, 
                               int(MAX_SEND_BUFFER_LENGTH - (sendBufCurrentPos - sendBuffer)), 0);
        if ( ret == -1 ) {
            std::cout << "Server send failed" << std::endl;
            sendBufCurrentPos = (char *) &ret;
        }

        IF_CONNECTION_CLOSED

        sendBufCurrentPos += ret;
    }

    PRESS_ANY_KEY

    closesocket(socket);
    
    return 0;
}