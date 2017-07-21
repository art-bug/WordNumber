#include <stdio.h>
#include <winsock2.h>
// Winsock2.h должен подключаться перед windows.h
#include <windows.h>
#include <locale.h>
#include <string>
#include <iostream>
#include <string.h>
#include <vector>
#include "word_number.h"

#pragma comment(lib, "ws2_32.lib")

#define PORT 5555     // Порт, который слушает сервер

#define PRESS_ANY_KEY std::cin.ignore(); std::cin.get();

DWORD WINAPI serviceActiveClient(LPVOID clientSocket);

int numActiveClients = 0;

Rank* pRanks[9] = {
    new One, new Ten, new Hundred, new Thousand, new TenThousand, new HundredThousand
};

// Выводит число активных пользователей
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

    // Инициализация библиотеки сокетов
    // Так как информация, возвращаемая функцией, не используется,
    // ей передается указатель на рабочий буфер, преобразуемый к 
    // указателю  на структуру WSADATA. Этот метод позволяет обойтись 
    // одной переменной, однако, буфер должен быть размером не менее 
    // полкилобайта (структура WSADATA занимает 400 байт)

    if (WSAStartup(0x0202, []() -> WSADATA* { WSADATA buf[400] = {0}; return buf; }() )) {
        std::cout << "Error WSAStartup " << WSAGetLastError() << std::endl;
        return -1;
    }

    // Создание сокета
    // AF_INET     - Интернет-сокет
    // SOCK_STREAM - потоковый сокет (с установкой соединения)
    // TCP протокол выбирается автоматически

    SOCKET sock = 0;
    if ( !(sock = socket(AF_INET, SOCK_STREAM, 0))) {
        std::cout << "Error socket " << WSAGetLastError() << std::endl;
        WSACleanup();
        // Деинициализация библиотеки Winsock
        return -1;
    }

    // Связка сокета с локальным адресом
    sockaddr_in localAddress = {};
    localAddress.sin_family = AF_INET;
    localAddress.sin_port = htons(PORT);
    localAddress.sin_addr.s_addr = 0;
    // Сервер принимает подключения на все IP адреса

    if ( bind(sock, (sockaddr *) &localAddress, sizeof(localAddress)) ) {
        std::cout << "Error bind " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    // Ожидание подключений
    // Размер очереди = 5
    if (listen(sock, 5)) {
        std::cout << "Error listen " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    std::cout << "Waiting for connections" << std::endl << std::endl;

    SOCKET clientSocket = 0;
    sockaddr_in clientAddress = {};

    int clientAddressSize = sizeof(clientAddress);

    // Цикл получения запросов на подключение из очереди
    while ( (clientSocket = accept(sock, (sockaddr *)
                            &clientAddress, &clientAddressSize)) )
    {
        // Вызов нового потока для обслуживания клиента
        // Для этого рекомендуется использовать _beginthreadex, 
        // но, поскольку никаких вызовов функций стандартной С библиотеки
        // поток не делает, можно обойтись CreateThread
        DWORD threadID = 0;
        CreateThread(0, 0, serviceActiveClient, &clientSocket, 0, &threadID);

        numActiveClients++;

        // Получение имени хоста
        HOSTENT *pHost = gethostbyaddr((char *) &clientAddress.sin_addr.s_addr, 4, AF_INET);

        printNumUsers();
    }

    return 0;
}

// Функция создается в отдельном потоке и обслуживает 
// подключившегося клиента независимо от остальных
DWORD WINAPI serviceActiveClient(LPVOID clientSocket) {
    SOCKET socket = ((SOCKET *) clientSocket)[0];
    
    //#define launched "Server is running\r\n"

    // приветствие 
    //send(socket, launched, sizeof(launched), 0);

    fd_set fd;
    timeval tv;
    tv.tv_sec = 5000;
    fd.fd_count = 1;
    fd.fd_array[0] = socket;

    // Прием числа и возврат клиенту его представления прописью
    const unsigned long MAX_RECEIVE_BUFFER_LENGTH = 8;
    char receiveBuffer[MAX_RECEIVE_BUFFER_LENGTH];
    char * receiveBufCurrentPos = receiveBuffer;

    #define IF_CONNECTION_CLOSED if ( ret == 0 ) {  \
                                    numActiveClients--;  \
                                    std::cout << "-disconnect" << std::endl;  \
                                    printNumUsers();  \
                                    closesocket(socket);  \
                                 }

    if ( select(1, &fd, NULL, NULL, &tv) != 0) {
        while( (unsigned long)(receiveBufCurrentPos - receiveBuffer) < MAX_RECEIVE_BUFFER_LENGTH+1 )
        {
            int ret = recv(socket, (char *) receiveBufCurrentPos, 
                                   int(MAX_RECEIVE_BUFFER_LENGTH - (receiveBufCurrentPos - receiveBuffer)), 0);
            if ( ret == -1 ) {
                std::cout << "server receive failed" << std::endl;
                break;
                return 0;
                //return ret;  ?
            }
    
            if ( ret == 0 ) {
                break;
            }
    
            IF_CONNECTION_CLOSED
    
            std::cout << "server receive ret  " << ret << std::endl;
    
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

    const unsigned long MAX_SEND_BUFFER_LENGTH = 1024;
    char sendBuffer[MAX_SEND_BUFFER_LENGTH] = {0};

    unsigned long number = std::stoi(receiveBufCurrentPos);

    number = ntohl(number);

    if (number == 0) {
        strcpy(sendBuffer, "ноль");
    }

    if (number < 0) {
        strcat(sendBuffer, "минус ");
        number = -number;
    }

    int numSize = int(log10(abs( (long)number)) + 1);

    const char * strNum = RuNum(number, pRanks[numSize-1]).format().c_str();

    strcat(sendBuffer, strNum);

    const char * sendBufCurrentPos = sendBuffer;

    if ( select(1, NULL, &fd, NULL, &tv) != 0) {
        while( (unsigned long)(sendBufCurrentPos - sendBuffer) < MAX_SEND_BUFFER_LENGTH )
        {
            int ret = send(socket, (const char *) sendBufCurrentPos, 
                                   int(MAX_SEND_BUFFER_LENGTH - (sendBufCurrentPos - sendBuffer)), 0);
            if ( ret == -1 ) {
                std::cout << "server send failed" << std::endl;
                break;
                return 0;
                //return ret;  ?
            }
    
            if ( ret == 0 ) {
                break;
            }
    
            IF_CONNECTION_CLOSED
    
            std::cout << "server send ret  " << ret << std::endl;
    
            sendBufCurrentPos += ret;
        }
    } else {
        return -1;
    }

    PRESS_ANY_KEY

    closesocket(socket);
    WSACleanup();

    return 0;
}