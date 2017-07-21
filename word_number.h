#ifndef WORD_NUMBER_H
#define WORD_NUMBER_H

#include <iostream>
#include <string>
#include <math.h>

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

struct Rank {
    virtual const std::string& format(int) = 0;
};

// Единицы
class One : public Rank {
    public:
        const std::string& format(int number) {
            return ranks[0][number-1];
        }
};

// Десятки
class Ten : public Rank {
    const std::string concreteTens[9] = {
        "одиннадцать ", "двенадцать ", "тринадцать ",
        "четырнадцать ", "пятнадцать ", "шестнадцать ",
        "семнадцать ", "восемнадцать ", "девятнадцать "
    };
    public:
        const std::string& format(int number) {
            int oneRankDigit = number % 10;
            number /= 10;
            if (number == 1) {
                return concreteTens[oneRankDigit-1];
            } else {
                return ranks[1][number-1] + ranks[0][oneRankDigit-1];
            }
        }
};

// Сотни
class Hundred : public Rank {
    public:
        const std::string& format(int number) {
            std::string resString = "";
            for (int period = 0; period < 3; period++) {
                resString.insert(0, ranks[period][ (number % 10) - 1 ]);
                number /= 10;
            }

            return resString;
        }
};

// Тысячи
class Thousand : public Rank {
    public:
        const std::string& format(int number) {
            std::string resString = "";
            for (int period = 0; period < 4; period++) {
                resString.insert(0, ranks[period][ (number % 10) - 1 ]);
                number /= 10;
            }

            return resString;
        }
};

// Десятки тысяч
class TenThousand : public Rank {
    public:
        const std::string& format(int number) {
            std::string resString = "";
            for (int period = 0; period < 5; period++) {
                resString.insert(0, ranks[period][ (number % 10) - 1 ]);
                number /= 10;
            }
            
            return resString;
        }
};

// Сотни тысяч
class HundredThousand : public Rank {
    public:
        const std::string& format(int number) {
            std::string resString = "";
            for (int period = 0; period < 6; period++) {
                resString.insert(0, ranks[period][ (number % 10) - 1 ]);
                number /= 10;
            }

            return resString;
        }
};

class RuNum {
    int number = 0;
    Rank *rank = 0;
    public:
        RuNum(int num, Rank *rank) {
            number = num;
            this->rank = rank;
        }

        ~RuNum() {
            delete rank;
        }

        const std::string& format() {
            return rank->format(number);
        }

        //friend std::ostream& operator << (std::ostream &, RuNum &);

        const std::string& operator = (const RuNum &);
};

/*std::ostream& operator << (std::ostream &os, RuNum &obj) {
        os << obj.format();
        return os;
}*/

#endif