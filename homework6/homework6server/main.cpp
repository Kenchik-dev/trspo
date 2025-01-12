#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <winsock2.h>
#include <string>
#include <cstring>

void sendBinaryMessage(SOCKET socket, const char* data, int length) {
    // Спочатку відправляємо довжину повідомлення (4 байти)
    send(socket, (char*)&length, sizeof(length), 0);
    // Потім відправляємо саме повідомлення
    send(socket, data, length, 0);
}

void receiveBinaryMessage(SOCKET socket, char* buffer, int& length) {
    // Спочатку отримуємо довжину повідомлення
    recv(socket, (char*)&length, sizeof(length), 0);
    // Потім отримуємо саме повідомлення
    recv(socket, buffer, length, 0);
    buffer[length] = '\0'; // Завершуємо рядок для коректного відображення
}

int main() {
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
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

    SOCKET newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr); // Приймаємо з'єднання

    if (newConnection == 0) {
        std::cout << "Error: failed to accept client." << std::endl;
        return 1;
    }
    else {
        std::cout << "Client connected." << std::endl;
    }

    char buffer[256];
    int length;

    for (int i = 0; i < 100; ++i) {
        // Отримуємо повідомлення від клієнта
        receiveBinaryMessage(newConnection, buffer, length);
        std::cout << "Received from client: " << buffer << std::endl;

        // Формуємо відповідь
        std::string serverMessage = "Server response #" + std::to_string(i + 1);
        sendBinaryMessage(newConnection, serverMessage.c_str(), serverMessage.size());
    }

    closesocket(newConnection);
    closesocket(sListen);
    WSACleanup();

    return 0;
}