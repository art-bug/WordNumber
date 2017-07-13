#include <iostream>
#include <string>
#include <math.h>
#include <locale.h>

template<class Type>
struct Formatter {
    virtual Type format() {}
};

template<int Rank>
class RuNum : public Formatter<std::string> {
    public:
        std::string format() const;

        ostream& operator << (ostream &os, RuNum<Rank> &obj) const {
              os << obj.format();
              return os;
        }

        std::string operator = (const RuNum<Rank> &obj) const {
              return obj.format();
        }
};

// Единицы
template<> class RuNum<1> {
    int number;
    public:
        RuNum(int number) {
            this.number = number;
        }

        std::string format() {
            std::string resString = "";
            if (number < 0) {
                resString += "минус ";
                number = -number;
            }
            switch (number) {
                case 1: resString += "один"; break;
                case 2: resString += "два"; break;
                case 3: resString += "три"; break;
                case 4: resString += "четыре"; break;
                case 5: resString += "пять"; break;
                case 6: resString += "шесть"; break;
                case 7: resString += "семь"; break;
                case 8: resString += "восемь"; break;
                case 9: resString += "девять"; break;
                default: return resString; break;
            }
        }
};

// Десятки
template<> class RuNum<2> {
    int number;
    public:
        RuNum(int number) {
            this.number = number;
        }

        std::string format() {
            std::string resString = "";
            std::string numStr = "";
            if (number < 0) {
                resString += "минус ";
                number = -number;
            }
            numStr = to_string(number);
            switch (numStr[0]) {
                case '1':
                    // Числа от 10 до 19
                    switch (numRanks[1]) {
                        case '0': resString += "десять "; break;
                        case '1': resString += "одиннадцать "; break;
                        case '2': resString += "двенадцать "; break;
                        case '3': resString += "тринадцать "; break;
                        case '4': resString += "четырнадцать "; break;
                        case '5': resString += "пятнадцать "; break;
                        case '6': resString += "шестнадцать "; break;
                        case '7': resString += "семнадцать "; break;
                        case '8': resString += "восемнадцать "; break;
                        case '9': resString += "девятнадцать "; break;
                    }
                break;
                case '2': resString += "двадцать "; break;
                case '3': resString += "тридцать "; break;
                case '4': resString += "сорок "; break;
                case '5': resString += "пятьдесят "; break;
                case '6': resString += "шестьдесят "; break;
                case '7': resString += "семьдесят "; break;
                case '8': resString += "восемьдесят "; break;
                case '9': resString += "девяносто "; break;
                default: break;
            }
            switch (numStr[1]) {
                case '1': resString += "один"; break;
                case '2': resString += "два"; break;
                case '3': resString += "три"; break;
                case '4': resString += "четыре"; break;
                case '5': resString += "пять"; break;
                case '6': resString += "шесть"; break;
                case '7': resString += "семь"; break;
                case '8': resString += "восемь"; break;
                case '9': resString += "девять"; break;
                default: return resString; break;
            }
        }
};

// Сотни
template<> class RuNum<3> {
    int number;
    public:
        RuNum(int number) {
            this.number = number;
        }

        std::string format() {
            std::string resString = "";
            std::string numStr = "";
            if (number < 0) {
                resString += "минус ";
                number = -number;
            }
            numStr = to_string(number);
            switch (numStr[0]) {
                case '1': resString += "сто "; break;
                case '2': resString += "двести "; break;
                case '3': resString += "триста "; break;
                case '4': resString += "четыреста "; break;
                case '5': resString += "пятьсот "; break;
                case '6': resString += "шестьсот "; break;
                case '7': resString += "семьсот "; break;
                case '8': resString += "восемьсот "; break;
                case '9': resString += "девятьсот "; break;
                default: break;
            }
            switch (numStr[1]) {
                case '1':
                    switch (numStr[2]) {
                        case '0': resString += "десять "; break;
                        case '1': resString += "одиннадцать "; break;
                        case '2': resString += "двенадцать "; break;
                        case '3': resString += "тринадцать "; break;
                        case '4': resString += "четырнадцать "; break;
                        case '5': resString += "пятнадцать "; break;
                        case '6': resString += "шестнадцать "; break;
                        case '7': resString += "семнадцать "; break;
                        case '8': resString += "восемнадцать "; break;
                        case '9': resString += "девятнадцать "; break;
                    }
                break;
                case '2': resString += "двадцать "; break;
                case '3': resString += "тридцать "; break;
                case '4': resString += "сорок "; break;
                case '5': resString += "пятьдесят "; break;
                case '6': resString += "шестьдесят "; break;
                case '7': resString += "семьдесят "; break;
                case '8': resString += "восемьдесят "; break;
                case '9': resString += "девяносто "; break;
                default: break;
            }
            switch (numStr[2]) {
                case '1': resString += "один"; break;
                case '2': resString += "два"; break;
                case '3': resString += "три"; break;
                case '4': resString += "четыре"; break;
                case '5': resString += "пять"; break;
                case '6': resString += "шесть"; break;
                case '7': resString += "семь"; break;
                case '8': resString += "восемь"; break;
                case '9': resString += "девять"; break;
                default: return resString; break;
            }
        }
};

// Тысячи
template<> class RuNum<4> {
    int number;
    public:
        RuNum(int number) {
            this.number = number;
        }

        std::string format() {
            std::string resString = "";
            std::string numStr = "";
            if (number < 0) {
                resString += "минус ";
                number = -number;
            }
            numStr = to_string(number);
            switch (numStr[0]) {
                case '1': resString += "одна тысяча "; break;
                case '2': resString += "две тысячи "; break;
                case '3': resString += "три тысячи "; break;
                case '4': resString += "четыре тысячи "; break;
                case '5': resString += "пять тысяч "; break;
                case '6': resString += "шесть тысяч "; break;
                case '7': resString += "семь тысяч "; break;
                case '8': resString += "восемь тысяч "; break;
                case '9': resString += "девять тысяч "; break;
                default: break;
            }
            switch (numStr[1]) {
                case '1': resString += "сто "; break;
                case '2': resString += "двести "; break;
                case '3': resString += "триста "; break;
                case '4': resString += "четыреста "; break;
                case '5': resString += "пятьсот "; break;
                case '6': resString += "шестьсот "; break;
                case '7': resString += "семьсот "; break;
                case '8': resString += "восемьсот "; break;
                case '9': resString += "девятьсот "; break;
                default: break;
            }
            switch (numStr[2]) {
                case '1':
                    switch (numStr[3]) {
                        case '0': resString += "десять "; break;
                        case '1': resString += "одиннадцать "; break;
                        case '2': resString += "двенадцать "; break;
                        case '3': resString += "тринадцать "; break;
                        case '4': resString += "четырнадцать "; break;
                        case '5': resString += "пятнадцать "; break;
                        case '6': resString += "шестнадцать "; break;
                        case '7': resString += "семнадцать "; break;
                        case '8': resString += "восемнадцать "; break;
                        case '9': resString += "девятнадцать "; break;
                    }
                break;
                case '2': resString += "двадцать "; break;
                case '3': resString += "тридцать "; break;
                case '4': resString += "сорок "; break;
                case '5': resString += "пятьдесят "; break;
                case '6': resString += "шестьдесят "; break;
                case '7': resString += "семьдесят "; break;
                case '8': resString += "восемьдесят "; break;
                case '9': resString += "девяносто "; break;
                default: break;
            }
            switch (numStr[3]) {
                case '1': resString += "один"; break;
                case '2': resString += "два"; break;
                case '3': resString += "три"; break;
                case '4': resString += "четыре"; break;
                case '5': resString += "пять"; break;
                case '6': resString += "шесть"; break;
                case '7': resString += "семь"; break;
                case '8': resString += "восемь"; break;
                case '9': resString += "девять"; break;
                default: return resString; break;
            }
        }
};

// Десятки тысяч
template<> class RuNum<5> {
    int number;
    public:
        RuNum(int number) {
            this.number = number;
        }

        std::string format() {
            std::string resString = "";
            std::string numStr = "";
            if (number < 0) {
                resString += "минус ";
                number = -number;
            }
            numStr = to_string(number);
            switch (numStr[0]) {
                case '1': resString += "десять тысяч "; break;
                case '2': resString += "двадцать тысяч "; break;
                case '3': resString += "тридцать тысячи "; break;
                case '4': resString += "сорок тысяч "; break;
                case '5': resString += "пятьдесят тысяч "; break;
                case '6': resString += "шестьдесят тысяч "; break;
                case '7': resString += "семьдесят тысяч "; break;
                case '8': resString += "восемьдесят тысяч "; break;
                case '9': resString += "девяносто тысяч "; break;
                default: break;
            }
            switch (numStr[1]) {
                case '1': resString += "одна тысяча "; break;
                case '2': resString += "две тысячи "; break;
                case '3': resString += "три тысячи "; break;
                case '4': resString += "четыре тысячи "; break;
                case '5': resString += "пять тысяч "; break;
                case '6': resString += "шесть тысяч "; break;
                case '7': resString += "семь тысяч "; break;
                case '8': resString += "восемь тысяч "; break;
                case '9': resString += "девять тысяч "; break;
                default: break;
            }
            switch (numStr[2]) {
                case '1': resString += "сто "; break;
                case '2': resString += "двести "; break;
                case '3': resString += "триста "; break;
                case '4': resString += "четыреста "; break;
                case '5': resString += "пятьсот "; break;
                case '6': resString += "шестьсот "; break;
                case '7': resString += "семьсот "; break;
                case '8': resString += "восемьсот "; break;
                case '9': resString += "девятьсот "; break;
                default: break;
            }
            switch (numStr[3]) {
                case '1':
                    switch (numStr[4]) {
                        case '0': resString += "десять "; break;
                        case '1': resString += "одиннадцать "; break;
                        case '2': resString += "двенадцать "; break;
                        case '3': resString += "тринадцать "; break;
                        case '4': resString += "четырнадцать "; break;
                        case '5': resString += "пятнадцать "; break;
                        case '6': resString += "шестнадцать "; break;
                        case '7': resString += "семнадцать "; break;
                        case '8': resString += "восемнадцать "; break;
                        case '9': resString += "девятнадцать "; break;
                    }
                break;
                case '2': resString += "двадцать "; break;
                case '3': resString += "тридцать "; break;
                case '4': resString += "сорок "; break;
                case '5': resString += "пятьдесят "; break;
                case '6': resString += "шестьдесят "; break;
                case '7': resString += "семьдесят "; break;
                case '8': resString += "восемьдесят "; break;
                case '9': resString += "девяносто "; break;
                default: break;
            }
            switch (numStr[4]) {
                case '1': resString += "один"; break;
                case '2': resString += "два"; break;
                case '3': resString += "три"; break;
                case '4': resString += "четыре"; break;
                case '5': resString += "пять"; break;
                case '6': resString += "шесть"; break;
                case '7': resString += "семь"; break;
                case '8': resString += "восемь"; break;
                case '9': resString += "девять"; break;
                default: return resString; break;
            }
        }
};

// Сотни тысяч
template<> class RuNum<6> {
    int number;
    public:
        RuNum(int number) {
            this.number = number;
        }

        std::string format() {
            std::string resString = "";
            std::string numStr = "";
            if (number < 0) {
                resString += "минус ";
                number = -number;
            }
            numStr = to_string(number);
            switch (numStr[0]) {
                case '1': resString += "сто тысяч "; break;
                case '2': resString += "двести тысяч "; break;
                case '3': resString += "триста тысяч "; break;
                case '4': resString += "четыреста тысяч "; break;
                case '5': resString += "пятьсот тысяч "; break;
                case '6': resString += "шестьсот тысяч "; break;
                case '7': resString += "семьсот тысяч "; break;
                case '8': resString += "восемьсот тысяч "; break;
                case '9': resString += "девятьсот тысяч "; break;
                default: break;
            }
            switch (numStr[1]) {
                case '1': resString += "десять тысяч "; break;
                case '2': resString += "двадцать тысяч "; break;
                case '3': resString += "тридцать тысячи "; break;
                case '4': resString += "сорок тысяч "; break;
                case '5': resString += "пятьдесят тысяч "; break;
                case '6': resString += "шестьдесят тысяч "; break;
                case '7': resString += "семьдесят тысяч "; break;
                case '8': resString += "восемьдесят тысяч "; break;
                case '9': resString += "девяносто тысяч "; break;
                default: break;
            }
            switch (numStr[2]) {
                case '1': resString += "одна тысяча "; break;
                case '2': resString += "две тысячи "; break;
                case '3': resString += "три тысячи "; break;
                case '4': resString += "четыре тысячи "; break;
                case '5': resString += "пять тысяч "; break;
                case '6': resString += "шесть тысяч "; break;
                case '7': resString += "семь тысяч "; break;
                case '8': resString += "восемь тысяч "; break;
                case '9': resString += "девять тысяч "; break;
                default: break;
            }
            switch (numStr[3]) {
                case '1': resString += "сто "; break;
                case '2': resString += "двести "; break;
                case '3': resString += "триста "; break;
                case '4': resString += "четыреста "; break;
                case '5': resString += "пятьсот "; break;
                case '6': resString += "шестьсот "; break;
                case '7': resString += "семьсот "; break;
                case '8': resString += "восемьсот "; break;
                case '9': resString += "девятьсот "; break;
                default: break;
            }
            switch (numStr[4]) {
                case '1':
                    switch (numStr[5]) {
                        case '0': resString += "десять "; break;
                        case '1': resString += "одиннадцать "; break;
                        case '2': resString += "двенадцать "; break;
                        case '3': resString += "тринадцать "; break;
                        case '4': resString += "четырнадцать "; break;
                        case '5': resString += "пятнадцать "; break;
                        case '6': resString += "шестнадцать "; break;
                        case '7': resString += "семнадцать "; break;
                        case '8': resString += "восемнадцать "; break;
                        case '9': resString += "девятнадцать "; break;
                    }
                break;
                case '2': resString += "двадцать "; break;
                case '3': resString += "тридцать "; break;
                case '4': resString += "сорок "; break;
                case '5': resString += "пятьдесят "; break;
                case '6': resString += "шестьдесят "; break;
                case '7': resString += "семьдесят "; break;
                case '8': resString += "восемьдесят "; break;
                case '9': resString += "девяносто "; break;
                default: break;
            }
            switch (numStr[5]) {
                case '1': resString += "один"; break;
                case '2': resString += "два"; break;
                case '3': resString += "три"; break;
                case '4': resString += "четыре"; break;
                case '5': resString += "пять"; break;
                case '6': resString += "шесть"; break;
                case '7': resString += "семь"; break;
                case '8': resString += "восемь"; break;
                case '9': resString += "девять"; break;
                default: return resString; break;
            }
        }
};

/*std::string RuNumber(int number) {
    std::string resString;
    if (number < 0) {
        resString += "минус ";
        number = -number;
    }

    int numRanks[6] = {0}; // массив разрядов
    int div = 100000; // делитель на разряды
    int var = number; // частное от деления
    for (int i = 0; i < 6; i++)
    {
        int rest = (int)(var / div);   // остаток от деления
        numRanks[i] = rest;
        var -= rest * div;
        div /= 10;
    }

    // Сотни тысяч
    switch (numRanks[0])
    {
    case 1: resString += "сто тысяч "; break;
    case 2: resString += "двести тысяч "; break;
    case 3: resString += "триста тысяч "; break;
    case 4: resString += "четыреста тысяч "; break;
    case 5: resString += "пятьсот тысяч "; break;
    case 6: resString += "шестьсот тысяч "; break;
    case 7: resString += "семьсот тысяч "; break;
    case 8: resString += "восемьсот тысяч "; break;
    case 9: resString += "девятьсот тысяч "; break;
    default: break;
    }

    //Десятки тысяч
    switch (numRanks[1])
    {
    case 1: resString += "десять тысяч "; break;
    case 2: resString += "двадцать тысяч "; break;
    case 3: resString += "тридцать тысячи "; break;
    case 4: resString += "сорок тысяч "; break;
    case 5: resString += "пятьдесят тысяч "; break;
    case 6: resString += "шестьдесят тысяч "; break;
    case 7: resString += "семьдесят тысяч "; break;
    case 8: resString += "восемьдесят тысяч "; break;
    case 9: resString += "девяносто тысяч "; break;
    default: break;
    }

    // Тысячи
    switch (numRanks[2])
    {
    case 1: resString += "одна тысяча "; break;
    case 2: resString += "две тысячи "; break;
    case 3: resString += "три тысячи "; break;
    case 4: resString += "четыре тысячи "; break;
    case 5: resString += "пять тысяч "; break;
    case 6: resString += "шесть тысяч "; break;
    case 7: resString += "семь тысяч "; break;
    case 8: resString += "восемь тысяч "; break;
    case 9: resString += "девять тысяч "; break;
    default: break;
    }

    // Сотни
    switch (numRanks[3])
    {
    case 1: resString += "сто "; break;
    case 2: resString += "двести "; break;
    case 3: resString += "триста "; break;
    case 4: resString += "четыреста "; break;
    case 5: resString += "пятьсот "; break;
    case 6: resString += "шестьсот "; break;
    case 7: resString += "семьсот "; break;
    case 8: resString += "восемьсот "; break;
    case 9: resString += "девятьсот "; break;
    default: break;
    }

    // Десятки
    switch (numRanks[4])
    {
    case 1:
        // Числа от 10 до 19
        switch (numRanks[5])
        {
        case 0: resString += "десять "; break;
        case 1: resString += "одиннадцать "; break;
        case 2: resString += "двенадцать "; break;
        case 3: resString += "тринадцать "; break;
        case 4: resString += "четырнадцать "; break;
        case 5: resString += "пятнадцать "; break;
        case 6: resString += "шестнадцать "; break;
        case 7: resString += "семнадцать "; break;
        case 8: resString += "восемнадцать "; break;
        case 9: resString += "девятнадцать "; break;
        }
        numRanks[5] = 0;
        break;
    case 2: resString += "двадцать "; break;
    case 3: resString += "тридцать "; break;
    case 4: resString += "сорок "; break;
    case 5: resString += "пятьдесят "; break;
    case 6: resString += "шестьдесят "; break;
    case 7: resString += "семьдесят "; break;
    case 8: resString += "восемьдесят "; break;
    case 9: resString += "девяносто "; break;
    default: break;
    }

    // Единицы
    switch (numRanks[5]) {
    case 1: resString += "один"; break;
    case 2: resString += "два"; break;
    case 3: resString += "три"; break;
    case 4: resString += "четыре"; break;
    case 5: resString += "пять"; break;
    case 6: resString += "шесть"; break;
    case 7: resString += "семь"; break;
    case 8: resString += "восемь"; break;
    case 9: resString += "девять"; break;
    default: break;
    }

    return resString;
}*/

int main() {
    setlocale(0, "RUS");
    int number;
    std::cout << "The number is ";
    std::cin >> number;
    RuNum<log10(abs(number)) + 1> ruNum(number);
    std::cout << " --> " << ruNum << std::endl;
    std::cin.get();
    return 0;
}