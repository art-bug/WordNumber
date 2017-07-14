#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <locale.h>

template<class Type>
struct Formatter {
    virtual Type format() {}
};

template<int Rank>
class RuNum : public Formatter<std::string> {
    public:
        std::string format() const;

        friend std::ostream& operator << (std::ostream &, const RuNum<Rank> &);

        std::string operator = (const RuNum<Rank> &obj) const {
              return obj.format();
        }
};

template<int Rank>
std::ostream& operator << (std::ostream &os, const RuNum<Rank> &obj) {
        os << obj.format();
        return os;
}

// Единицы
template<> class RuNum<1> {
    int number;
    public:
        std::string ones[9] = {
            "один", "два", "три", 
            "четыре", "пять", "шесть", 
            "семь", "восемь", "девять"
        };

        RuNum<1>(int number) {
            this->number = number;
        }

        std::string format() {
            return ones[number-1];
        }
};

// Десятки
template<> class RuNum<2> {
    int number;
    public:
        std::string tens[9] = {
            "десять ", "двадцать ", "тридцать ", 
            "сорок ", "пятьдесят ", "шестьдесят ", 
            "семьдесят ", "восемьдесят ", "девяносто "
        };

        std::string concreteTens[9] = { 
            "одиннадцать ", "двенадцать ", "тринадцать ", 
            "четырнадцать ", "пятнадцать ", "шестнадцать ",
            "семнадцать ", "восемнадцать ", "девятнадцать "
        };

        RuNum<2>(int number) {
            this->number = number;
        }

        std::string format() {
            int oneRankDigit = number % 10;
            number /= 10;
            if (number == 1) {
                return concreteTens[oneRankDigit-1];
            } else {
                return tens[number-1] + RuNum<1>(oneRankDigit).format();
            }
        }
};

// Сотни
template<> class RuNum<3> {
    int number;
    public:
        std::string hundreds[9] = { 
            "сто ", "двести ", "триста ", 
            "четыреста ", "пятьсот ", "шестьсот ", 
            "семьсот ", "восемьсот ", "девятьсот "
        };

        RuNum<3>(int number) {
            this->number = number;
        }

        std::string format() {
            int oneRankDigit = number % 10;
            number /= 10;
            int tenRankDigit = number % 10;
            number /= 10;
            return hundreds[number-1] + RuNum<2>(tenRankDigit).format() + RuNum<1>(oneRankDigit).format();
        }
};

// Тысячи
template<> class RuNum<4> {
    int number;
    public:
        std::string thousands[9] = {
            "одна тысяча ", "две тысячи ", "три тысячи ", 
            "четыре тысячи ", "пять тысяч ", "шесть тысяч ", 
            "семь тысяч ", "восемь тысяч ", "девять тысяч ",
        };

        RuNum<4>(int number) {
            this->number = number;
        }

        std::string format() {
            std::vector<int> digits;
            std::string resString = "";
            for (int digit = 0; digit < 4; digit++) {
                digits.push_back(number % 10);
                number /= 10;
            }

            return thousands[ digits[3] ] + RuNum<3>( digits[2] ).format() + 
                   RuNum<2>( digits[1] ).format() + RuNum<1>( digits[0] ).format();
        }
};

// Десятки тысяч
template<> class RuNum<5> {
    int number;
    public:
        std::string ten_thousands[9] = {
            "десять тысяч ", "двадцать тысяч ", "тридцать тысяч ", 
            "сорок тысяч ", "пятьдесят тысяч ", "шестьдесят тысяч ",
            "семьдесят тысяч ", "восемьдесят тысяч ", "девяносто тысяч "
        };

        RuNum<5>(int number) {
            this->number = number;
        }

        std::string format() {
            std::vector<int> digits;
            std::string resString = "";
            for (int digit = 0; digit < 5; digit++) {
                digits.push_back(number % 10);
                number /= 10;
            }

            return ten_thousands[ digits[4] ] + RuNum<4>( digits[3] ).format() + 
                   RuNum<3>( digits[2] ).format() + RuNum<2>( digits[1] ).format() + 
                   RuNum<1>( digits[0] ).format();
        }
};

// Сотни тысяч
template<> class RuNum<6> {
    int number;
    public:
        std::string hundred_thousands[9] = {
            "сто тысяч ", "двести тысяч ", "триста тысяч ",
            "четыреста тысяч ", "пятьсот тысяч ", "шестьсот тысяч ",
            "семьсот тысяч ", "восемьсот тысяч ", "девятьсот тысяч "
        };

        RuNum<6>(int number) {
            this->number = number;
        }

        std::string format() {
            std::vector<int> digits;
            for (int digit = 0; digit < 6; digit++) {
                digits.push_back(number % 10);
                number /= 10;
            }

            return hundred_thousands[ digits[5] ] + RuNum<5>( digits[4] ).format() + 
                   RuNum<4>( digits[3] ).format() + RuNum<3>( digits[2] ).format() +
                   RuNum<2>( digits[1] ).format() + RuNum<1>( digits[0] ).format();
        }
};

int main() {
    setlocale(0, "RUS");
    int number;
    std::cout << "The number is ";
    std::cin >> number;
    std::cout << " --> ";
    if (number == 0) {
        std::cout << "ноль" << std::endl;
        std::cin.get();
        return 0;
    }
    if (number < 0) {
        std::cout << "минус ";
        number = -number;
    }
    RuNum<log10(abs(number)) + 1> ruNum(number);
    std::cout << ruNum << std::endl;
    std::cin.get();
    return 0;
}