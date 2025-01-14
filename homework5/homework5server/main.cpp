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

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0); // Створюємо сокет
    bind(sListen, (SOCKADDR*)&addr, sizeof(addr)); // Прив'язуємо сокет до адреси
    listen(sListen, SOMAXCONN); // Очікуємо клієнта

    std::cout << "The server is ready to receive clients...\n";

    SOCKET newConnection;
    newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr); // Приймаємо з'єднання

    if (newConnection == 0)
    {
        std::cout << "Error: failed to accept client." << std::endl;
        return 1;
    }
    else
    {
        std::cout << "Client connected." << std::endl;
    }

    char buffer[256]; // Буфер для прийому даних
    while (true)
    {
        // Прийом повідомлення від клієнта
        int msgLength = recv(newConnection, buffer, sizeof(buffer), 0);
        if (msgLength > 0)
        {
            buffer[msgLength] = '\0'; // Завершуємо строку
            std::cout << "Client: " << buffer << std::endl;

            // Перевірка на завершення
            if (std::string(buffer) == "0")
            {
                std::cout << "Client disconnected." << std::endl;
                break;
            }
        }

        // Введення та відправка повідомлення клієнту
        std::string serverMessage;
        std::cout << "Server: ";
        std::getline(std::cin, serverMessage);
        send(newConnection, serverMessage.c_str(), serverMessage.size(), 0);

        // Перевірка на завершення
        if (serverMessage == "0")
        {
            std::cout << "Server shutting down..." << std::endl;
            break;
        }
    }

    closesocket(newConnection);
    closesocket(sListen);
    WSACleanup();

    return 0;
}
