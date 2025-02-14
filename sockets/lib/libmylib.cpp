#include "mylib.h"
#include <algorithm>
#include <cctype>

namespace MyLib {

    void sortAndReplace(std::string &input) {
        // Сортировка символов по убыванию
        std::sort(input.begin(), input.end(), std::greater<char>());
        std::string result;
        for (char ch : input) {
            // Если символ – цифра и она чётная
            if (std::isdigit(ch) && ((ch - '0') % 2 == 0)) {
                result += "KV"; // Замена чётной цифры на "KV"
            } else {
                result.push_back(ch);
            }
        }
        input = result;
    }

    int sumNumericValues(const std::string &input) {
        int sum = 0;
        for (char ch : input) {
            if (std::isdigit(ch)) {
                sum += ch - '0';
            }
        }
        return sum;
    }

    bool analyzeString(const std::string &input) {
        int count = 0;
        for (char ch : input) {
            if (std::isalnum(static_cast<unsigned char>(ch))) {
                ++count;
            }
        }
        return (count > 2 && count % 32 == 0);
    }
}

