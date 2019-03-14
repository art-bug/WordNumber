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

        ushort getNumSize(int number)
		{
			return log10(abs((long double) number)) + 1;
		}

        std::string runNext(int number)
        {
            if (!_nextRank) {
                return "";
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
	
		virtual std::string format(int number) = 0;

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

        std::string format(int number)
		{
            return ranks[0][number-1];
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

        std::string format(int number)
		{
            if (getNumSize(number) < 2) {
                return runNext(number);
            }

            ushort oneRankDigit = number % 10;
			ushort tenRankDigit = number / 10;

            if (tenRankDigit == 1) {
				if (oneRankDigit == 0) {
					return ranks[1][0];
				}
				
                return concreteTens[oneRankDigit-1];
            }
			
			if (oneRankDigit == 0) {
				return ranks[1][tenRankDigit - 1];
			}

			return std::string(ranks[1][tenRankDigit - 1]) + runNext(oneRankDigit);
        }
};

// Hundreds
class Hundred : public Rank
{
    public:

        std::string format(int number)
		{
            if (getNumSize(number) < 3) {
                return runNext(number);
            }

            ushort tenNumber = number % 100;
            ushort hundredDigit = number / 100;
			
			if (tenNumber == 0) {
				return ranks[2][hundredDigit - 1];
			}
			
            return std::string(ranks[2][hundredDigit - 1]) + runNext(tenNumber);
        }
};

// Thousands
class Thousand : public Rank
{
    public:

        std::string format(int number)
		{
            if (getNumSize(number) < 4) {
                return runNext(number);
            }

            ushort hundredNumber = number % 1000;
            ushort thousandDigit = number / 1000;
			
			if (hundredNumber == 0) {
				return ranks[3][thousandDigit - 1];
			}

            return std::string(ranks[3][thousandDigit - 1]) + runNext(hundredNumber);
        }
};

// Tens of thousands
class TenThousand : public Rank
{
    public:

        std::string format(int number)
		{
            if (getNumSize(number) < 5) {
                return runNext(number);
            }
            
			ushort hundredNumber = number % 1000;
            ushort tenThousandNumber = number / 1000;
			
			if (tenThousandNumber % 10 == 0) {
				if (hundredNumber == 0) {
					return ranks[4][tenThousandNumber / 10 - 1];
				}
				
				return ranks[4][tenThousandNumber / 10 - 1] + runNext(hundredNumber);
			}
			
			ushort thousandNumber = number % 10000;
			
            return runNext(tenThousandNumber / 10 * 10) + runNext(thousandNumber);
        }
};

// Hundreds of thousands
class HundredThousand : public Rank
{
    public:

        std::string format(int number)
		{
			if (getNumSize(number) > 6) {
				return "Число с данной разрядностью не поддерживается.";
			}
			
            if (getNumSize(number) < 6) {
                return runNext(number);
            }
			
			ushort hundredThousandNumber = number / 1000;
			ushort hundredNumber = number % 1000;
			
			if (hundredThousandNumber % 100 == 0 && hundredNumber == 0) {
				return ranks[5][hundredThousandNumber / 100 - 1];
			}
			else {
				if (hundredThousandNumber % 100 == 0) {
					return ranks[5][hundredThousandNumber / 100 - 1] + runNext(hundredNumber);
				}
				
				if (hundredNumber == 0) {
					return runNext(hundredThousandNumber / 100 * 100) + runNext(number % 100000);
				}
			}
			
			ushort thousandNumber = number % 10000;

            return runNext(hundredThousandNumber / 10 * 10) + runNext(thousandNumber);
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

        std::string format(int number)
		{   
            return rootRank->format(number);
        }
};

static WordNumber wnServer;

#endif