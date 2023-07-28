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
#include "Braces.h"

#include <cstring>

#ifdef DEBUG
#include <cstdio>
#endif // DEBUG

Braces::Braces()
{

};
Braces::~Braces()
{
    this->wipe();
};

int  Braces::parse (const char * str, const char * obr, const char * cbr, bool avoid_quotes)
{
    this->wipe();

    bool _avoid_qoutes = false;
    if (avoid_quotes)
    {
        qt.parse(str);
        if (qt.get_len() < 0) return -1;
        else if (qt.get_len() > 0) _avoid_qoutes = true;
    }

    const char * p;

    int qnt_bo = 0;
    int qnt_bc = 0;

    for (int n = 0; n < (int)strlen(obr); n++)
    {
        p = strchr(str, *(obr + n));
        while (p != nullptr)
        {
            if (_avoid_qoutes)
            {
                for (int m = 0; m < qt.get_len(); m++)
                {
                    if (((p - str) > qt.arr_open[m]) && ((p - str) < qt.arr_close[m]))
                    {
                        qnt_bo--;
                        break;
                    }
                }
            }
            qnt_bo++;
            p = strchr(p + 1, *(obr + n));
        }
    }

    for (int n = 0; n < (int)strlen(cbr); n++)
    {
        p = strchr(str, *(cbr + n));
        while (p != nullptr)
        {
            if (_avoid_qoutes)
            {
                for (int m = 0; m < qt.get_len(); m++)
                {
                    if (((p - str) > qt.arr_open[m]) && ((p - str) < qt.arr_close[m]))
                    {
                        qnt_bc--;
                        break;
                    }
                }
            }
            qnt_bc++;
            p = strchr(p + 1, *(cbr + n));
        }
    }


    if (qnt_bo != qnt_bc)  return -1;
    if (qnt_bo == 0)
    {
        this->len = 0;
        return 0;
    }

    this->len = qnt_bo;
    //allocate mem
    arr_open =   new int [this->len];
    arr_close =  new int [this->len];
    arr_types =  new int [this->len];


    int arr_bo_tmp[this->len], arr_bc_tmp[this->len], arr_tp_tmp[this->len];


    int n = 0;
    bool exclude = false;

    for (int i = 0; i < NUM_TYPES; i++)
    {
        if (strchr(obr, this->openBr[i]) == nullptr)
        {
            continue;
        }

        p = strchr(str, this->openBr[i]);
        while (p != nullptr)
        {
            if (_avoid_qoutes)
            {
                for (int m = 0; m < qt.get_len(); m++)
                {
                    if (((p - str) > qt.arr_open[m]) && ((p - str) < qt.arr_close[m]))
                    {
                        exclude = true;
                        break;
                    }
                }
            }
            if (!exclude)
            {
                arr_bo_tmp[n] = p - str;
                arr_tp_tmp[n] = i;
                if (n++ == this->len) return -1;
            }
            else exclude = false;

            p = strchr(p + 1, this->openBr[i]);
        }
    }

    n = 0;
    exclude = false;
    for (int i = 0; i < NUM_TYPES; i++)
    {
        if (strchr(cbr, this->closeBr[i]) == nullptr)
        {
            continue;
        }

        p = strchr(str, this->closeBr[i]);
        while (p != nullptr)
        {
            if (_avoid_qoutes)
            {
                for (int m = 0; m < qt.get_len(); m++)
                {
                    if (((p - str) > qt.arr_open[m]) && ((p - str) < qt.arr_close[m]))
                    {
                        exclude = true;
                        break;
                    }
                }
            }
            if (!exclude)
            {
                arr_bc_tmp[n] = p - str;
                if (n++ == this->len) return -1;
            }
            else exclude = false;

            p = strchr(p + 1, this->closeBr[i]);
        }
    }

    int cursor = 0;

    int tc, to, cl_pos, op_pos;
    for (int i = 0; i < this->len; i++)
    {
        cl_pos = arr_bc_tmp[i];
        tc = arr_tp_tmp[i];

        for  (int j = this->len - 1; j >= 0; j--)
        {
            to = arr_tp_tmp[j];
            op_pos = arr_bo_tmp[j];
            //Compare types of braces
            if (tc != to) continue;
            if (op_pos == -1) continue;
            //Compare positions
            if (op_pos > cl_pos) continue;

            arr_bo_tmp[j] = -1;
            arr_types[cursor] = tc;
            arr_open[cursor] = op_pos;
            arr_close[cursor] = cl_pos;
            cursor++;
            break;
        }
    }


    return 0;
};

int  Braces::order_asc()
{
    if (!this->arr_open) return -1;

    int res, pos, buf;

    for (int j = 0; j < this->len - 1; j++)
    {

        if (arr_open[j] < arr_open[1+j])
        {
            res = arr_open[j];
            pos = j;
        }
        else
        {
            res = arr_open[1+j];
            pos = 1+j;
        }
        for (int i = 2+j; i < this->len; i++)
        {
            if (res > arr_open[i])
            {
                res = arr_open[i];
                pos = i;
            }
        }
        buf = arr_open[j];
        arr_open[j] = arr_open[pos];
        arr_open[pos] = buf;

        buf = arr_close[j];
        arr_close[j] = arr_close[pos];
        arr_close[pos] = buf;

        buf = arr_types[j];
        arr_types[j] = arr_types[pos];
        arr_types[pos] = buf;
    }
    return 0;
};

int Braces::deep_lvl()
{
    if (!this->arr_open) return -1;

    arr_deep_lvl = new int[this->len];

    int deep_lvl;

    for (int n = 0; n < this->len; n++)
    {
        deep_lvl=0;

        for (int m = 0; m < n; m++)
        {
            if (n == 0) break;
            if (!((this->arr_open[n] > this->arr_open[m]) && (this->arr_open[n] > this->arr_close[m]))) deep_lvl++;
        }

        this->arr_deep_lvl[n] = deep_lvl;

        this->deep = (deep_lvl > this->deep) ? deep_lvl : this->deep;

    }
    return 0;
};

int Braces::get_len()
{
    return this->len;
};

int Braces::get_deep_lvl()
{
    return this->deep;
};

void Braces::wipe()
{
    if (arr_open != nullptr)
        delete []arr_open;

    if (arr_close != nullptr)
        delete []arr_close;

    if (arr_types != nullptr)
        delete []arr_types;

    if (arr_deep_lvl != nullptr)
        delete []arr_deep_lvl;

    arr_open = arr_close = arr_types = arr_deep_lvl = nullptr;

    this->len = -1;
    return;
}

#ifdef DEBUG
void Braces::show()
{
    if (arr_open)
    {
        printf("\n\nBrace positions:\n");
        for (int i = 0; i < this->len; i++)
        {
            printf("t:%d\t%c%d - %d%c\t\tdeep: %d\n", arr_types[i], openBr[arr_types[i]], arr_open[i], arr_close[i], closeBr[arr_types[i]], (arr_deep_lvl) ? arr_deep_lvl[i] : -1);
        }
        printf("\n\n");
    }
    else
    {
        printf("\n\nERROR.\nEmpty arr.\n");
    }
}
#endif
