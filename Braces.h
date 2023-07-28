/*MIT License

Copyright (c) 2023 Aleksandr Yuschenko

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef BRACES_H_INCLUDED
#define BRACES_H_INCLUDED

#include "Quotes.h"

/*Braces types*/
#define     NUM_TYPES   4

#define     CURVY       0   //{}
#define     SQUARE      1   //[]
#define     ROUND       2   //()
#define     TRIANGLE    3   //<>

#define DEBUG

class Braces
{
public:
    Braces();
    ~Braces();

    Quotes qt;

    int* arr_types = nullptr;
    int* arr_open = nullptr;
    int* arr_close = nullptr;
    int* arr_deep_lvl = nullptr;


    int parse (const char * str, const char * obr, const char * cbr, bool avoid_quotes = false);
    int order_asc();
    int deep_lvl();

    int get_len();
    int get_deep_lvl();

    void wipe();

    #ifdef DEBUG
    void show();
    #endif // DEBUG

private:

    int len = 0;
    int deep = -1;

    char openBr[NUM_TYPES] = {'{', '[', '(', '<'};
    char closeBr[NUM_TYPES] = {'}', ']', ')', '>'};
};


#endif // BRACES_H_INCLUDED
