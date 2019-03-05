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
	Rank *_nextRank { nullptr };

    protected:

        ushort numSize {0};

        char* runNext(int number)
        {
            if (!_nextRank) {
                return (char*)"";
			}

            return _nextRank->format(number);
        }

	public:

		void setNext(Rank *nextRank)
		{
            if (!_nextRank) {
                _nextRank = nextRank;
            }
            else {
                _nextRank->setNext(nextRank);
            }
		}
	
		virtual char* format(int number) = 0;

		virtual ~Rank()
        {
            if (_nextRank) {
                delete _nextRank;
            }
        }
};

// Ones
class One : public Rank
{
    public:

        char* format(int number)
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

        char* format(int number)
		{
            numSize = int(log10(abs((long double) number)) + 1);

            if (numSize < 2) {
                return runNext(number);
            }

            ushort oneRankDigit = number % 10;
			ushort tenRankDigit = number / 10;

            if (tenRankDigit == 1) {
                return strcat((char*)concreteTens[oneRankDigit-1], runNext(oneRankDigit));
            }

			return strcat((char*)ranks[1][tenRankDigit - 1], runNext(oneRankDigit));
        }
};

// Hundreds
class Hundred : public Rank
{
    public:

        char* format(int number)
		{
            numSize = int(log10(abs((long double) number)) + 1);

            if (numSize < 3) {
                return runNext(number);
            }

            ushort tenNumber = number % 100;
            ushort hundredDigit = number / 100;

			return strcat((char*)ranks[2][hundredDigit - 1], runNext(tenNumber));
        }
};

// Thousands
class Thousand : public Rank
{
    public:

        char* format(int number)
		{
            numSize = int(log10(abs((long double) number)) + 1);

            if (numSize < 4) {
                return runNext(number);
            }

            ushort hundredNumber = number % 1000;
            ushort thousandDigit = number / 10000;

			return strcat((char*)ranks[3][thousandDigit - 1], runNext(hundredNumber));
        }
};

// Tens of thousands
class TenThousand : public Rank
{
    public:

        char* format(int number)
		{
            numSize = int(log10(abs((long double) number)) + 1);

            if (numSize < 5) {
                return runNext(number);
            }
            
            ushort thousandNumber = number % 10000;
            ushort tenThousandDigit = number / 100000;

			return strcat((char*)ranks[4][tenThousandDigit - 1], runNext(thousandNumber));
        }
};

// Hundreds of thousands
class HundredThousand : public Rank
{
    public:

        char* format(int number)
		{
            numSize = int(log10(abs((long double) number)) + 1);

            if (numSize < 6) {
                return runNext(number);
            }

            ushort tenThousandNumber = number % 100000;
            ushort hundredThousandDigit = number / 1000000;

			return strcat((char*)ranks[5][hundredThousandDigit - 1], runNext(tenThousandNumber));
        }
};

class WordNumber
{
    Rank *rootRank {nullptr};

    Rank* rankPointers[6] = {
        new HundredThousand(),
        new TenThousand(),
        new Thousand(),
        new Hundred(),
        new Ten(),
        new One()
    };

    void initRankChain()
    {
	    for (auto rankPtr : rankPointers) {
	        installRank(rankPtr);
	    }
    }

    public:

        WordNumber()
        {
            initRankChain();
        }

        void installRank(Rank *rank)
        {
            if (!rootRank) {
                rootRank = rank;
            }
            else {
                rootRank->setNext(rank);
            }
        }

        char* format(int number)
		{
            return rootRank->format(number);
        }
};

static WordNumber wnServer;

// std::map<ushort, Rank*> rankChain;

// void initRankChain() {
// 	Rank* rankClassPointers[] = {
// 		new One(),
// 		new Ten(),
// 		new Hundred(),
// 		new Thousand(),
// 		new TenThousand(),
// 		new HundredThousand()
// 	};

// 	ushort strLen = 1;
// 	for (auto rankClassPointer : rankClassPointers) {
// 		rankChain[strLen] = rankClassPointer;
// 		++strLen;
// 	}

// 	while (strLen > 1) {
// 		rankChain[strLen - 1]->setNext(rankChain[strLen - 1]);
// 		--strLen;
// 	}
// }

#endif