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
#include "Quotes.h"

#include <cstring>

#ifdef DEBUG
#include <cstdio>
#endif // DEBUG

Quotes::Quotes()
{
//constr
};
Quotes::~Quotes()
{
    this->wipe();
};
int Quotes::parse (const char * str)
{
    this->wipe();

    int qnt = 0;

    const char * pos;
    pos = strchr(str, '"');
    while (pos != nullptr)
    {
        if (*(pos - 1) != 0x5C) qnt ++;
        pos = strchr(pos + 1, '"');
    }

    if (qnt == 0)
    {
        this->len = 0;
        return 0;
    }
    else if (qnt%2 != 0) return -1;
    else this->len = qnt/2;


    arr_open  = new int[this->len];
    arr_close  = new int[this->len];


    bool flag = true;
    int n = 0;
    pos = strchr(str, '"');

    while (pos != nullptr)
    {
        if (*(pos - 1) != 0x5C)
        {
            if (flag)
                arr_open[n] = pos - str;
            else
            {
                arr_close[n] = pos - str;
                n++;
            }
            flag = !flag;
        };
        pos = strchr(pos + 1, '"');
    }
    return 0;
};

void Quotes::wipe()
{

    if (arr_open)
    {
        delete[] arr_open;
        arr_open = nullptr;
    }
    if (arr_close)
    {
        delete[] arr_close;
        arr_close = nullptr;
    }
    this->len = -1;
    return;
}

int Quotes::get_len()
{
    return this->len;
};

#ifdef DEBUG
void Quotes::show()
{
    printf("\nQoutes\n\n");
    for (int i = 0; i < this->len; i++)
        printf("n:%d\t[%d - %d]\n",i, arr_open[i], arr_close[i]);

    return;
};
#endif
