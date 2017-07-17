#include <iostream>
#include <string>
#include <math.h>
#include <locale.h>

template<class Type>
struct Formatter {
    virtual Type format() {}
};

static const std::string ranks[6][9] = {
        {
            "один", "два", "три", 
            "четыре", "пять", "шесть", 
            "семь", "восемь", "девять"
        },
        {
            "десять ", "двадцать ", "тридцать ", 
            "сорок ", "пятьдесят ", "шестьдесят ", 
            "семьдесят ", "восемьдесят ", "девяносто "
        },
        { 
            "сто ", "двести ", "триста ", 
            "четыреста ", "пятьсот ", "шестьсот ", 
            "семьсот ", "восемьсот ", "девятьсот "
        },
        {
            "одна тысяча ", "две тысячи ", "три тысячи ", 
            "четыре тысячи ", "пять тысяч ", "шесть тысяч ", 
            "семь тысяч ", "восемь тысяч ", "девять тысяч "
        },
        {
            "десять тысяч ", "двадцать тысяч ", "тридцать тысяч ", 
            "сорок тысяч ", "пятьдесят тысяч ", "шестьдесят тысяч ",
            "семьдесят тысяч ", "восемьдесят тысяч ", "девяносто тысяч "
        },
        {
            "сто тысяч ", "двести тысяч ", "триста тысяч ",
            "четыреста тысяч ", "пятьсот тысяч ", "шестьсот тысяч ",
            "семьсот тысяч ", "восемьсот тысяч ", "девятьсот тысяч "
        }
};

template<int Rank>
class RuNum : public Formatter<std::string> {
    public:
        const std::string& format();

        friend std::ostream& operator << (std::ostream &, RuNum<Rank> &);

        const std::string& operator = (const RuNum<Rank> &);
};

template<int Rank>
std::ostream& operator << (std::ostream &os, RuNum<Rank> &obj) {
        os << obj.format();
        return os;
}

// Единицы
template<> class RuNum<1> {
    int number = 0;
    public:
        RuNum(int num) {
            number = num;
        }

        const std::string& format() {
            return ranks[0][number-1];
        }
};

// Десятки
template<> class RuNum<2> {
    int number = 0;
    const std::string concreteTens[9] = { 
        "одиннадцать ", "двенадцать ", "тринадцать ", 
        "четырнадцать ", "пятнадцать ", "шестнадцать ",
        "семнадцать ", "восемнадцать ", "девятнадцать "
    };
    public:
        RuNum(int num) {
            number = num;
        }

        const std::string& format() {
            int oneRankDigit = number % 10;
            number /= 10;
            if (number == 1) {
                return concreteTens[oneRankDigit-1];
            } else {
                return ranks[1][number-1] + ranks[0][oneRankDigit-1];
            }
        }
};

#define runum_specialize(PERIOD) template<> class RuNum<(PERIOD)> {      \
    int number = 0;                                                      \
    public:                                                              \
        RuNum(int num) {                                                 \
            number = num;                                                \
        }                                                                \
                                                                         \
        const std::string& format() {                                    \
            std::string resString = "";                                  \
            for (int period = 0; period < (PERIOD); period++) {          \
                resString.insert(0, ranks[period][ (number % 10) - 1 ]); \
                number /= 10;                                            \
            }                                                            \
                                                                         \
            return resString;                                            \
        }                                                                \
};

// Сотни
runum_specialize(3)

// Тысячи
runum_specialize(4)

// Десятки тысяч
runum_specialize(5)

// Сотни тысяч
runum_specialize(6)

int main() {
    setlocale(0, "RUS");
    /*int number = 0;
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
    numDigits = int(log10(abs(number)) + 1);
    RuNum<numDigits> ruNum(number);*/
    RuNum<4> ruNum(3456);
    std::cout << ruNum << std::endl;
    std::cin.get();
    return 0;
}