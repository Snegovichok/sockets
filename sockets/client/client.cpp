#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <algorithm>
#include <cctype>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "mylib.h"  // Подключаем заголовок динамической библиотеки

// Общий буфер и объекты синхронизации
std::string sharedBuffer;
std::mutex mtx;
std::condition_variable cv;
bool dataAvailable = false;
bool exitFlag = false;

const char* SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 54000;

void inputThreadFunc() {
    while (!exitFlag) {
        std::string input;
        std::cout << "Введите строку (только цифры, не более 64 символов): ";
        std::getline(std::cin, input);
        
        // Валидация ввода
        if (input.empty()) {
            std::cout << "Ошибка: строка не должна быть пустой." << std::endl;
            continue;
        }
        if (input.length() > 64) {
            std::cout << "Ошибка: строка не должна превышать 64 символа." << std::endl;
            continue;
        }
        if (!std::all_of(input.begin(), input.end(), ::isdigit)) {
            std::cout << "Ошибка: строка должна состоять только из цифр." << std::endl;
            continue;
        }
        
        // Обработка строки через функцию 1 из библиотеки
        MyLib::sortAndReplace(input);
        
        // Запись в общий буфер
        {
            std::unique_lock<std::mutex> lock(mtx);
            // Если данные ещё не обработаны, ждём освобождения буфера
            cv.wait(lock, [](){ return !dataAvailable; });
            sharedBuffer = input;
            dataAvailable = true;
        }
        cv.notify_all();
    }
}

void processingThreadFunc() {
    while (!exitFlag) {
        std::string data;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [](){ return dataAvailable || exitFlag; });
            if (exitFlag) break;
            data = sharedBuffer;
        }
        
        std::cout << "Обработанная строка: " << data << std::endl;
        
        // Получение суммы цифр через функцию 2
        int sum = MyLib::sumNumericValues(data);
        std::cout << "Сумма цифр в строке: " << sum << std::endl;
        
        // Попытка установить TCP-соединение с сервером
        int sockfd;
        while (true) {
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0) {
                std::cerr << "Ошибка создания сокета." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                continue;
            }
            
            sockaddr_in serverAddr;
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_port = htons(SERVER_PORT);
            if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
                std::cerr << "Ошибка преобразования IP-адреса." << std::endl;
                close(sockfd);
                std::this_thread::sleep_for(std::chrono::seconds(2));
                continue;
            }
            
            if (connect(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
                std::cerr << "Не удалось подключиться к серверу. Попытка переподключения..." << std::endl;
                close(sockfd);
                std::this_thread::sleep_for(std::chrono::seconds(2));
                continue;
            }
            break; // Соединение установлено
        }
        
        // Отправка данных серверу
        ssize_t sent = send(sockfd, data.c_str(), data.size(), 0);
        if (sent < 0) {
            std::cerr << "Ошибка при отправке данных." << std::endl;
        } else {
            std::cout << "Данные отправлены на сервер: " << data << std::endl;
        }
        close(sockfd);
        
        // Очистка общего буфера
        {
            std::lock_guard<std::mutex> lock(mtx);
            sharedBuffer.clear();
            dataAvailable = false;
        }
        cv.notify_all();
    }
}

int main() {
    std::thread t1(inputThreadFunc);
    std::thread t2(processingThreadFunc);
    
    t1.join();
    t2.join();
    
    return 0;
}

