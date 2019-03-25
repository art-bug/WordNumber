#ifndef WORD_NUMBER_H
#define WORD_NUMBER_H

#include <iostream>
#include <string>
#include <math.h>

using ushort = unsigned short;

class Rank
{
	Rank *_nextRank { nullptr };

    protected:

        uint8_t getNumSize(int number)
		{
			return log10(abs(number)) + 1;
		}

        std::wstring runNext(int number)
        {
            if (!_nextRank) {
                return L"";
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
	
		virtual std::wstring format(int number) = 0;

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
	const wchar_t* ones[9] = {
		L"один", L"два", L"три",
        L"четыре", L"пять", L"шесть",
        L"семь", L"восемь", L"девять"
	};
	
    public:

        std::wstring format(int number)
		{
            return ones[number-1];
        }
};

// Tens
class Ten : public Rank
{
	const wchar_t* tens[9] = {
		L"десять ", L"двадцать ", L"тридцать ",
        L"сорок ", L"пятьдесят ", L"шестьдесят ",
        L"семьдесят ", L"восемьдесят ", L"девяносто "
	};
	
	const wchar_t* concreteTens[9] = {
		L"одиннадцать ", L"двенадцать ", L"тринадцать ",
		L"четырнадцать ", L"пятнадцать ", L"шестнадцать ",
		L"семнадцать ", L"восемнадцать ", L"девятнадцать "
	};

    public:

        std::wstring format(int number)
		{
            if (getNumSize(number) < 2) {
                return runNext(number);
            }

            ushort oneRankDigit = number % 10;
			ushort tenRankDigit = number / 10;

            if (tenRankDigit == 1) {
				if (oneRankDigit == 0) {
					return tens[0];
				}
				
                return concreteTens[oneRankDigit-1];
            }
			
			if (oneRankDigit == 0) {
				return tens[tenRankDigit - 1];
			}

			return std::wstring(tens[tenRankDigit - 1]) + runNext(oneRankDigit);
        }
};

// Hundreds
class Hundred : public Rank
{
	const wchar_t* hundreds[9] = {
		L"сто ", L"двести ", L"триста ",
        L"четыреста ", L"пятьсот ", L"шестьсот ",
        L"семьсот ", L"восемьсот ", L"девятьсот "
	};
	
    public:

        std::wstring format(int number)
		{
            if (getNumSize(number) < 3) {
                return runNext(number);
            }

            ushort tenNumber = number % 100;
            ushort hundredDigit = number / 100;
			
			if (tenNumber == 0) {
				return hundreds[hundredDigit - 1];
			}
			
            return std::wstring(hundreds[hundredDigit - 1]) + runNext(tenNumber);
        }
};

// Thousands
class Thousand : public Rank
{
	const wchar_t* thousands[9] = {
		L"одна тысяча ", L"две тысячи ", L"три тысячи ",
        L"четыре тысячи ", L"пять тысяч ", L"шесть тысяч ",
        L"семь тысяч ", L"восемь тысяч ", L"девять тысяч "
	};
	
    public:

        std::wstring format(int number)
		{
            if (getNumSize(number) < 4) {
                return runNext(number);
            }

            ushort hundredNumber = number % 1000;
            ushort thousandDigit = number / 1000;
			
			if (hundredNumber == 0) {
				return thousands[thousandDigit - 1];
			}

            return std::wstring(thousands[thousandDigit - 1]) + runNext(hundredNumber);
        }
};

// Tens of thousands
class TenThousand : public Rank
{
	const wchar_t* tensOfThousands[9] = {
		L"десять тысяч ", L"двадцать тысяч ", L"тридцать тысяч ",
        L"сорок тысяч ", L"пятьдесят тысяч ", L"шестьдесят тысяч ",
        L"семьдесят тысяч ", L"восемьдесят тысяч ", L"девяносто тысяч "
	};
	
	const wchar_t* concreteTensOfThousands[9] = {
		L"одиннадцать тысяч ", L"двенадцать тысяч ", L"тринадцать тысяч ",
        L"четырнадцать тысяч ", L"пятнадцать тысяч ", L"шестнадцать тысяч ",
        L"семнадцать тысяч ", L"восемнадцать тысяч ", L"девятнадцать тысяч "
	};
	
    public:

        std::wstring format(int number)
		{
            if (getNumSize(number) < 5) {
                return runNext(number);
            }
            
			ushort hundredNumber = number % 1000;
            ushort tenThousandNumber = number / 1000;
			ushort thousandDigit = tenThousandNumber % 10;
			
			if (thousandDigit == 0 && hundredNumber == 0) {
				return tensOfThousands[tenThousandNumber / 10 - 1];
			}
			else {
				if (thousandDigit == 0) {
					return tensOfThousands[tenThousandNumber / 10 - 1] + runNext(hundredNumber);
				}
				
                if (hundredNumber == 0) {
				    if (tenThousandNumber / 10 == 1 && thousandDigit > 0) {
				    	return concreteTensOfThousands[thousandDigit - 1];
				    }
                }
			}

            if (tenThousandNumber / 10 == 1 && thousandDigit > 0) {
				return concreteTensOfThousands[thousandDigit - 1] + runNext(hundredNumber);
			}

            ushort thousandNumber = number % 10000;
			
            return runNext(tenThousandNumber / 10 * 10) + runNext(thousandNumber);
        }
};

// Hundreds of thousands
class HundredThousand : public Rank
{
	const wchar_t* hundredsOfThousands[9] = {
		L"сто тысяч ", L"двести тысяч ", L"триста тысяч ",
        L"четыреста тысяч ", L"пятьсот тысяч ", L"шестьсот тысяч ",
        L"семьсот тысяч ", L"восемьсот тысяч ", L"девятьсот тысяч "
	};
	
    public:

        std::wstring format(int number)
		{
            if (getNumSize(number) < 6) {
                return runNext(number);
            }
			
			ushort hundredThousandNumber = number / 1000;
			ushort hundredNumber = number % 1000;
			
			if (hundredThousandNumber % 100 == 0 && hundredNumber == 0) {
				return hundredsOfThousands[hundredThousandNumber / 100 - 1];
			}
			else {
				if (hundredThousandNumber % 100 == 0) {
					return hundredsOfThousands[hundredThousandNumber / 100 - 1] + runNext(hundredNumber);
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

        std::wstring format(int number)
		{
            if (number == 0) {
                return L"ноль";
            }

            if ((uint8_t) log10(abs(number)) + 1 > 6) {
                return L"Число с данной разрядностью не поддерживается.";
            }

            if (number < 0) {
                return L"минус " + rootRank->format(-number);
            }

            return rootRank->format(number);
        }
};

static WordNumber wnServer;

#endif