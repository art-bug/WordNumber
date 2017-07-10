#include <iostream>
#include <string>
#include <map>
#include <locale.h>

using namespace std;

map<char, string> ones = {
                            { '1', "один" },
                            { '2', "два" },
                            { '3', "три" },
                            { '4', "четыре" },
                            { '5', "пять" },
                            { '6', "шесть" },
                            { '7', "семь" },
                            { '8', "восемь" },
                            { '9', "девять" }
                         };

string ten_chunk = "десят";

map<string, string> tens = {
                            { "10", "десять" },
                            { "20", ones['2'] + "дцать" },
                            { "30", ones['3'] + "дцать" },
                            { "40", "сорок" },
                            { "50", ones['5'] + ten_chunk },
                            { "60", ones['6'] + ten_chunk },
                            { "70", ones['7'] + ten_chunk },
                            { "80", ones['8'] + ten_chunk },
                            { "90", "девяносто" }
                           };

string hundred_chunk = "сот";

map<string, string> hundreds = {
                                { "100", "сто" },
                                { "200", "двести" },
                                { "300", "триста" },
                                { "400", "четыреста" },
                                { "500", ones['5'] + hundred_chunk },
                                { "600", ones['6'] + hundred_chunk },
                                { "700", ones['7'] + hundred_chunk },
                                { "800", ones['8'] + hundred_chunk },
                                { "900", ones['9'] + hundred_chunk },
                               };

string thousands_chunk = "тысяч";

map<string, string> hundreds = {
                                { "1000", "одна " + thousands_chunk + 'а' },
                                { "2000", "две "  + thousands_chunk + 'и' },
                                { "3000", ones['3'] + ' ' + thousands_chunk + 'и' },
                                { "4000", ones['4'] + ' ' + thousands_chunk + 'и' },
                                { "5000", ones['5'] + ' ' + thousands_chunk },
                                { "6000", ones['6'] + ' ' + thousands_chunk },
                                { "7000", ones['7'] + ' ' + thousands_chunk },
                                { "8000", ones['8'] + ' ' + thousands_chunk },
                                { "9000", ones['9'] + ' ' + thousands_chunk }
                               }

int main() {
    setlocale(0, "RUS");
    return 0;
}