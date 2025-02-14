#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <chrono>
#include <thread>

#include "mylib.h"  // Подключаем заголовок динамической библиотеки

const int SERVER_PORT = 54000;

int main() {
    while (true) {
        // Создание TCP-сокета
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) {
            std::cerr << "Ошибка создания сокета." << std::endl;
            return 1;
        }
        
        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            std::cerr << "Ошибка установки опций сокета." << std::endl;
            close(server_fd);
            return 1;
        }
        
        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr("127.0.0.1"); // фиксированный IP
        address.sin_port = htons(SERVER_PORT);
        
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
            std::cerr << "Ошибка привязки сокета к порту." << std::endl;
            close(server_fd);
            return 1;
        }
        
        if (listen(server_fd, 3) < 0) {
            std::cerr << "Ошибка прослушивания порта." << std::endl;
            close(server_fd);
            return 1;
        }
        
        std::cout << "Сервер запущен, ожидается подключение..." << std::endl;
        
        socklen_t addrlen = sizeof(address);
        int new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (new_socket < 0) {
            std::cerr << "Ошибка при принятии подключения." << std::endl;
            close(server_fd);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            continue;
        }
        
        std::cout << "Подключение установлено с " 
                  << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port) 
                  << std::endl;
        
        char buffer[1024] = {0};
        ssize_t valread = recv(new_socket, buffer, sizeof(buffer) - 1, 0);
        if (valread <= 0) {
            std::cerr << "Ошибка получения данных или соединение разорвано." << std::endl;
            close(new_socket);
            close(server_fd);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            continue;
        }
        
        buffer[valread] = '\0';
        std::string receivedData(buffer);
        std::cout << "Получены данные: " << receivedData << std::endl;
        
        // Анализ полученных данных через функцию 3
        bool analysisResult = MyLib::analyzeString(receivedData);
        if (analysisResult) {
            std::cout << "Данные корректны." << std::endl;
        } else {
            std::cout << "Ошибка: некорректные данные." << std::endl;
        }
        
        close(new_socket);
        close(server_fd);
        std::cout << "Ожидание нового подключения..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    
    return 0;
}

