#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>
// Winsock2.h must be connected before windows.h
#include <windows.h>
#include <stdlib.h>
#include <locale.h>
#include "word_number.h"

#define PORT 5555
#define SERVERADDRESS "127.0.0.1"

#define PRESS_ANY_KEY std::cin.ignore(); std::cin.get();

using ulong = unsigned long;

void fail() {
    std::cout << "FAIL" << std::endl;
    PRESS_ANY_KEY
}

int main() {
    setlocale(LC_ALL, "RUS");

    std::cout << "NUMTOSTR DEMO CLIENT" << std::endl;

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

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (!sock) {
        std::cout << "Socket error " << WSAGetLastError() << std::endl;
		WSACleanup();
        PRESS_ANY_KEY
        return -1;
    }

	// Specify the server address and port

    sockaddr_in dest_addr = {};
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);

	// Converting an IP address from a character format to a network
    if (inet_addr(SERVERADDRESS) != INADDR_NONE) {
        dest_addr.sin_addr.s_addr = inet_addr(SERVERADDRESS);
    } else {
		HOSTENT *pHost = 0;

		// Obtain an IP address by the domain name of the server
        if (pHost = gethostbyname(SERVERADDRESS)) {

			//      Important!
			// pHost-> h_addr_list contains an array of pointers to addresses
            ((ulong *) &dest_addr.sin_addr)[0] =
                           ((ulong **) pHost->h_addr_list)[0][0];

        } else {
            std::cout << "Invalid address " << SERVERADDRESS << std::endl;
            PRESS_ANY_KEY
            closesocket(sock);
            WSACleanup();
            return -1;
        }
    }

    // Establish the connection
    if (connect(sock, (sockaddr *) &dest_addr, sizeof(dest_addr))) {
        fail();
        return -1;
    }

    #define IF_CONNECTION_CLOSED if (!ret) {  \
                                    std::cout << "Connection closed" << std::endl; \
                                    return -1;  \
                                 }

    const ulong MAX_SEND_BUF_SIZE = 8;
    char sendBuffer[MAX_SEND_BUF_SIZE] = {0};

    ulong number = 0;

    std::cin >> number;

    ushort numSize = int(log10(abs( (long double)number )) + 1);

    const char* strNum = rankChain[numSize]->format(number);

    number = htonl(number);

    sprintf(sendBuffer, "%d", number);

    const char* sendBufCurrentPos = sendBuffer;

    while( sendBufCurrentPos - sendBuffer < MAX_SEND_BUF_SIZE ) 
    {
        int ret = send(sock, sendBufCurrentPos, 
                             int(MAX_SEND_BUF_SIZE - (sendBufCurrentPos - sendBuffer)), 0);
        if ( ret == -1 ) {
            std::cout << "client send failed" << std::endl;
            sendBufCurrentPos = (char *) &ret;
        }

        IF_CONNECTION_CLOSED

        sendBufCurrentPos += ret;
    }

    const ulong MAX_RECEIVE_BUFFER_LENGTH = 1024;
    char receiveBuffer[MAX_RECEIVE_BUFFER_LENGTH];
    char* receiveBufCurrentPos = receiveBuffer;

    while( (ulong)(receiveBufCurrentPos - receiveBuffer) < MAX_RECEIVE_BUFFER_LENGTH )
    {
        int ret = recv(sock, (char *) receiveBufCurrentPos, 
                             int(MAX_RECEIVE_BUFFER_LENGTH - (receiveBufCurrentPos - receiveBuffer)), 0);
        if ( ret == -1 ) {
            std::cout << "client receive failed" << std::endl;
            receiveBufCurrentPos = (char *) &ret;
        }

        IF_CONNECTION_CLOSED

        receiveBufCurrentPos += ret;

        if ( (ulong)(receiveBufCurrentPos - receiveBuffer) < MAX_RECEIVE_BUFFER_LENGTH ) 
        {
            std::cout << "[recv notice] attempt to receive " << MAX_RECEIVE_BUFFER_LENGTH << " bytes but " 
            << (ulong)(receiveBufCurrentPos - receiveBuffer) << " received per call (" << 
            (MAX_RECEIVE_BUFFER_LENGTH - ((ulong)(receiveBufCurrentPos - receiveBuffer))) << " left to receive)"
            << std::endl;
        }  
    }

    closesocket(sock);

    if (strcmp(strNum, receiveBuffer) == 0) {    
        std::cout << " --> " << receiveBuffer << std::endl;

        PRESS_ANY_KEY

    } else {
        fail();
    }

    WSACleanup();

    return 0;
}