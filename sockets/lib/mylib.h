#ifndef MYLIB_H
#define MYLIB_H

#include <string>

namespace MyLib {

    // Функция 1:
    // Сортирует символы строки по убыванию и заменяет каждый символ,
    // являющийся чётной цифрой, на строку "KV".
    void sortAndReplace(std::string &input);

    // Функция 2:
    // Суммирует все цифры, найденные в строке.
    int sumNumericValues(const std::string &input);

    // Функция 3:
    // Анализирует строку: если количество алфавитно-цифровых символов > 2 и кратно 32,
    // возвращает true, иначе – false.
    bool analyzeString(const std::string &input);
}

#endif // MYLIB_H

