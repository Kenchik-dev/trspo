#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <winsock2.h> // Ліба для використання WinSock2

int main()
{
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if(WSAStartup(DLLVersion, &wsaData) != 0)
    {
        std::cout << "Error: failed to initialize WinSock." << std::endl;
    }

    // Заповнюємо інформацію про адресу сокета
    SOCKADDR_IN addr; // Структура для зберігання адреси
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // "127.0.0.1" - localhost
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET;

    SOCKET Connection = socket(AF_INET, SOCK_STREAM, 0); // Створюємо новий сокет для з'єднання з сервером

    if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0)
    {
        std::cout << "Error: failed connect to server.\n";
        return 1;
    }
    std::cout << "Connected to server." << std::endl;

    system("pause");

    return 0;
}
