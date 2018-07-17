#ifndef WORD_NUMBER_H
#define WORD_NUMBER_H

#include <iostream>
#include <map>
#include <string.h>
#include <math.h>

using ushort = unsigned short;

const char* ranks[6][9] = {
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

class Rank 
{
	Rank *next;

	public:

		Rank() : next(nullptr) {}

		void setNext(Rank *rank)
		{
			next = rank;
		}
	
		virtual char* format(int number)
		{
			if (next) {
				return next->format(number);
			}

			return (char*)"";
		}

		virtual ~Rank() {}
};

// Ones
class One : public Rank 
{
    public:

        char* format(int number) override
		{
            return (char*)ranks[0][number-1];
        }
};

// Tens
class Ten : public Rank
{
	const char* concreteTens[9] = {
		"одиннадцать ", "двенадцать ", "тринадцать ",
		"четырнадцать ", "пятнадцать ", "шестнадцать ",
		"семнадцать ", "восемнадцать ", "девятнадцать "
	};

    public:

        char* format(int number) override
		{
            ushort oneRankDigit = number % 10;
			ushort tenRankDigit = (number % 100) / 10;

            if (tenRankDigit == 1) {
                return strcat((char*)concreteTens[oneRankDigit-1], Rank::format(number));
            } 

			return strcat((char*)ranks[1][tenRankDigit - 1], Rank::format(number));
        }
};

// Hundreds
class Hundred : public Rank 
{
    public:

        char* format(int number) override
		{
            char* resString = new char();
            for (int period = 2; period >= 0; period--) {
                strcat(resString, ranks[period][ (number % 10) - 1 ]);
                number /= 10;
            }

			return strcat(resString, Rank::format(number));
        }
};

// Thousands
class Thousand : public Rank
{
    public:

        char* format(int number) override
		{
            char* resString = new char();
            for (int period = 3; period >= 0; period--) {
                strcat(resString, ranks[period][ (number % 10) - 1 ]);
                number /= 10;
            }

			return strcat(resString, Rank::format(number));
        }
};

// Tens of thousands
class TenThousand : public Rank 
{
    public:

        char* format(int number) override
		{
            char* resString = new char();
            for (int period = 4; period >= 0; period--) {
                strcat(resString, ranks[period][ (number % 10) - 1 ]);
                number /= 10;
            }
            
            return strcat(resString, Rank::format(number));
        }
};

// Hundreds of thousands
class HundredThousand : public Rank 
{
    public:

        char* format(int number) override
		{
            char* resString = new char();
            for (int period = 5; period >= 0; period--) {
                strcat(resString, ranks[period][ (number % 10) - 1 ]);
                number /= 10;
            }

			return strcat(resString, Rank::format(number));
        }
};

std::map<ushort, Rank*> rankChain;

void initRankChain() {
	Rank* rankClassPointers[] = {
		new One(),
		new Ten(),
		new Hundred(),
		new Thousand(),
		new TenThousand(),
		new HundredThousand()
	};

	ushort strLen = 1;
	for (auto rankClassPointer : rankClassPointers) {
		rankChain[strLen] = rankClassPointer;
		++strLen;
	}

	while (strLen > 1) {
		rankChain[strLen - 1]->setNext(rankChain[strLen - 1]);
		--strLen;
	}
}

#endif