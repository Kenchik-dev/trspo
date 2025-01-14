#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <winsock2.h>
#include <string>

int main()
{
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsaData) != 0)
    {
        std::cout << "Error: failed to initialize WinSock." << std::endl;
        return 1;
    }

    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost
    addr.sin_port = htons(1111); // Порт для з'єднання
    addr.sin_family = AF_INET;

    SOCKET Connection = socket(AF_INET, SOCK_STREAM, 0); // Створюємо сокет

    if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) // Підключаємось до сервера
    {
        std::cout << "Error: failed to connect to server." << std::endl;
        return 1;
    }
    std::cout << "Connected to server." << std::endl;

    char buffer[256]; // Буфер для прийому даних
    while (true)
    {
        // Введення та відправка повідомлення серверу
        std::string clientMessage;
        std::cout << "Client: ";
        std::getline(std::cin, clientMessage);
        send(Connection, clientMessage.c_str(), clientMessage.size(), 0);

        // Перевірка на завершення
        if (clientMessage == "0")
        {
            std::cout << "Client shutting down..." << std::endl;
            break;
        }

        // Прийом повідомлення від сервера
        int msgLength = recv(Connection, buffer, sizeof(buffer), 0);
        if (msgLength > 0)
        {
            buffer[msgLength] = '\0'; // Завершуємо строку
            std::cout << "Server: " << buffer << std::endl;

            // Перевірка на завершення
            if (std::string(buffer) == "0")
            {
                std::cout << "Server disconnected." << std::endl;
                break;
            }
        }
    }

    closesocket(Connection);
    WSACleanup();

    return 0;
}
