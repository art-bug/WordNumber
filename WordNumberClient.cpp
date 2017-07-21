#include <stdio.h>
#include <string.h>
#include <winsock2.h>
// Winsock2.h должен подключаться перед windows.h
#include <windows.h>
#include <stdlib.h>
#include <string>
#include <locale.h>
#include <iostream>
#include "word_number.h"

#pragma comment(lib, "ws2_32.lib")

#define PORT 5555
#define SERVERADDRESS "127.0.0.1"

Rank* pRanks[9] = {
    new One, new Ten, new Hundred, new Thousand, new TenThousand, new HundredThousand
};

#define PRESS_ANY_KEY std::cin.ignore(); std::cin.get();

void fail() {
    std::cout << "FAIL" << std::endl;
    PRESS_ANY_KEY
}

int main() {
    setlocale(LC_ALL, "RUS");

    std::cout << "NUMTOSTR DEMO CLIENT" << std::endl << std::endl;

    // Инициализация библиотеки Winsock
    // Так как информация, возвращаемая функцией, не используется,
    // ей передается указатель на рабочий буфер, преобразуемый к 
    // указателю на структуру WSADATA. Этот метод позволяет обойтись 
    // одной переменной, однако, буфер должен быть размером не менее 
    // полкилобайта (структура WSADATA занимает 400 байт)

    if ( WSAStartup(0x0202, []() -> WSADATA* { WSADATA buf[400] = {0}; return buf; }() )) {
        std::cout << "WSAStart error " << WSAGetLastError() << std::endl;
        PRESS_ANY_KEY
        return -1;
    }

    // Создание сокета
    // AF_INET     - Интернет-сокет
    // SOCK_STREAM - потоковый сокет (с установкой соединения)
    // TCP протокол выбирается автоматически

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (!sock) {
        std::cout << "Socket() error " << WSAGetLastError() << std::endl;
        PRESS_ANY_KEY
        return -1;
    }

    // Установка соединения
    // Заполнение структуры sockaddr_in
    // Указание адреса и порта сервера

    sockaddr_in dest_addr = {};
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);
    HOSTENT *pHost = 0;

    // Конвертация IP адреса из символьного формата в сетевой
    if (inet_addr(SERVERADDRESS) != INADDR_NONE) {
        dest_addr.sin_addr.s_addr = inet_addr(SERVERADDRESS);
    } else {
        // Получение IP адреса по доменному имени сервера
        if (pHost = gethostbyname(SERVERADDRESS)) {

            //      Важно!
            // pHost->h_addr_list содержит массив указателей на адреса
            ((unsigned long *) &dest_addr.sin_addr)[0] =
                           ((unsigned long **) pHost->h_addr_list)[0][0];

        } else {
            std::cout << "Invalid address " << SERVERADDRESS << std::endl;
            PRESS_ANY_KEY
            closesocket(sock);
            WSACleanup();
            return -1;
        }
    }

    fd_set fd;
    timeval tv;
    tv.tv_sec = 5000;
    fd.fd_count = 1;
    fd.fd_array[0] = sock;

    // Установка соединения
    if (connect(sock, (sockaddr *) &dest_addr, sizeof(dest_addr))) {
        fail();
        return -1;
    }

    #define IF_CONNECTION_CLOSED if ( ret == 0 ) {  \
                                    std::cout <<  \
                                         "connection closed"  \
                                         << std::endl; \
                                    break;          \
                                    PRESS_ANY_KEY  \
                                    closesocket(sock);  \
                                    WSACleanup();       \
                                    return -1;  \
                                  }

    const unsigned long MAX_SEND_BUF_SIZE = 8;
    char sendBuffer[MAX_SEND_BUF_SIZE] = {0};

    unsigned long number = 0;

    std::cin >> number;

    int numSize = int(log10(abs( (long)number )) + 1);

    const char * strNum = RuNum(number, pRanks[numSize-1]).format().c_str();

    number = htonl(number);

    strcpy(sendBuffer, std::to_string(number).c_str());

    const char * sendBufCurrentPos = sendBuffer;

    if ( select(1, NULL, &fd, NULL, &tv) != 0 ) {
        while( (unsigned long)(sendBufCurrentPos - sendBuffer) < MAX_SEND_BUF_SIZE+1 )
        {
            int ret = send(sock, (const char *) sendBufCurrentPos, 
                                 int(MAX_SEND_BUF_SIZE - (sendBufCurrentPos - sendBuffer)), 0);
            if ( ret == -1 ) {
                std::cout << "client send failed" << std::endl;
                PRESS_ANY_KEY
                closesocket(sock);
                WSACleanup();
                //return ret;  ?
            }

            if ( ret == 0 ) {
                break;
            }

            IF_CONNECTION_CLOSED

            std::cout << "client send ret  " << ret << std::endl;

            sendBufCurrentPos += ret;
        }
    } else {
        return -1;
    }

    const unsigned long MAX_RECEIVE_BUFFER_LENGTH = 1024;
    char receiveBuffer[MAX_RECEIVE_BUFFER_LENGTH];
    char * receiveBufCurrentPos = receiveBuffer;

    if ( select(1, &fd, NULL, NULL, &tv) != 0 ) {
        while( (unsigned long)(receiveBufCurrentPos - receiveBuffer) < MAX_RECEIVE_BUFFER_LENGTH+1 )
        {
            int ret = recv(sock, (char *) receiveBufCurrentPos, 
                                 int(MAX_RECEIVE_BUFFER_LENGTH - (receiveBufCurrentPos - receiveBuffer)), 0);
            if ( ret == -1 ) {
                std::cout << "client receive failed" << std::endl;
                PRESS_ANY_KEY
                closesocket(sock);
                WSACleanup();
                //return ret;  ?
            }
    
            if ( ret == 0 ) {
                break;
            }
    
            IF_CONNECTION_CLOSED
    
            std::cout << "client receive ret  " << ret << std::endl;
    
            receiveBufCurrentPos += ret;
    
            if ( (unsigned long)(receiveBufCurrentPos - receiveBuffer) < MAX_RECEIVE_BUFFER_LENGTH ) 
            {
                std::cout << "[recv notice] attempt to receive " << MAX_RECEIVE_BUFFER_LENGTH << " bytes but " 
                << (unsigned long)(receiveBufCurrentPos - receiveBuffer) << " received per call (" << 
                (MAX_RECEIVE_BUFFER_LENGTH - ((unsigned long)(receiveBufCurrentPos - receiveBuffer))) << " left to receive)"
                << std::endl;
            }
        }
    } else {
        return -1;
    }

    std::cout << " --> " << receiveBuffer << std::endl;

    PRESS_ANY_KEY

    closesocket(sock);
    WSACleanup();

    return 0;
}