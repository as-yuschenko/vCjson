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
#include "vCjson.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>


vCjson::vCjson() {};

vCjson::~vCjson()
{
    wipe_all();
};


int vCjson::get_num_nodes()
{
    return _qnt_nodes;
};

int vCjson::get_node_type()
{
    return (_curr_node) ? _curr_node->type : -1 ;
};

int vCjson::get_node_type_by_name(const char* name)
{
    if (!go_node_name(name)) return _curr_node->type;
    else return -1;
};

int vCjson::go_node_name (const char* name)
{
    if (_qnt_nodes > 0)
    {
        if (name)
        {
            int name_len = strlen(name);

            for (int i = 0; i < _qnt_nodes; i++)
            {
                if (!_arr_nodes[i].name_len) continue;
                if (name_len != _arr_nodes[i].name_len) continue;

                if (!strncmp(name, _arr_nodes[i].name, name_len))
                {
                    _curr_parent_id = _arr_nodes[i].parent_id;
                    _curr_self_id  = _arr_nodes[i].self_id;
                    _curr_node = &_arr_nodes[i];
                    return 0;
                }
            }
        }
    }
    return -1;
};

int vCjson::go_node_name_on_layer(const char* name)
{
    if (_qnt_nodes > 0)
    {
        if (name)
        {
            int name_len = strlen(name);

            for (int i = 0; i < _qnt_nodes; i++)
            {
                if (_curr_parent_id != _arr_nodes[i].parent_id) continue;
                if (!_arr_nodes[i].name_len) continue;
                if (name_len != _arr_nodes[i].name_len) continue;

                if (!strncmp(name, _arr_nodes[i].name, name_len))
                {
                    _curr_parent_id = _arr_nodes[i].parent_id;
                    _curr_self_id  = _arr_nodes[i].self_id;
                    _curr_node = &_arr_nodes[i];
                    return 0;
                }
            }
        }
    }
    return -1;
}

int vCjson::get_sibling_num()
{
    int n = 0;
    for (int i = 0; i < _qnt_nodes; i++)
        if (_arr_nodes[i].parent_id == _curr_parent_id)
            n++;
    return n;
};

int vCjson::go_next_sibling()
{
    if (_qnt_nodes > 0)
    {
        for (int i = _curr_self_id; i < _qnt_nodes; i++)
        {
            if (_arr_nodes[i].parent_id == _curr_parent_id)
            {
                if (_arr_nodes[i].self_id > _curr_self_id)
                {
                    _curr_self_id = _arr_nodes[i].self_id;
                    _curr_node = &_arr_nodes[i];
                    return 0;
                }
            }
        }
    }
    return -1;
};

int vCjson::go_prev_sibling()
{
    if (_qnt_nodes > 0)
    {
        if (_curr_self_id > 0)
        {
            for (int i = _curr_self_id - 1; i > -1; i--)
            {
                if (_arr_nodes[i].parent_id == _curr_parent_id)
                {
                    if (_arr_nodes[i].self_id < _curr_self_id)
                    {
                        _curr_node = &_arr_nodes[i];
                        _curr_self_id = _arr_nodes[i].self_id;
                        return 0;
                    }
                }
            }
        }
    }
    return -1;
};

int vCjson::go_node_child()
{
    if (_qnt_nodes > 0)
    {
        if (_curr_self_id)
        {
            for (int i = _curr_self_id; i < _qnt_nodes; i++)
            {
                if (_arr_nodes[i].parent_id == _curr_self_id)
                {
                    _curr_parent_id =_curr_self_id;
                    _curr_self_id = 0;
                    _curr_node = nullptr;
                    return 0;
                }
            }
        }
    }
    return -1;
};

int vCjson::go_node_parent()
{
    if (_qnt_nodes > 0)
    {
        if (_curr_parent_id != 0)
        {
            for (int i = _curr_parent_id - 1; i > -1; i++)
            {
                if (_arr_nodes[i].self_id == _curr_parent_id)
                {
                    _curr_node = &_arr_nodes[i];
                    _curr_self_id = _arr_nodes[i].self_id;
                    _curr_parent_id = _arr_nodes[i].parent_id;
                    return 0;
                }
            }
        }
    }
    return -1;
};

void vCjson::go_root()
{
    _curr_node = nullptr;
    _curr_parent_id = 0;
    _curr_self_id = 0;
}

char* vCjson::get_node_name_ptr()
{
    return (_curr_node) ? _curr_node->name : nullptr;
};

int vCjson::get_node_name_len()
{
    return (_curr_node) ? _curr_node->name_len : 0;
};

char* vCjson::get_node_value_ptr()
{
    return (_curr_node) ? _curr_node->value : nullptr;
};

int vCjson::get_node_value_len()
{
    return (_curr_node) ? _curr_node->value_len : 0 ;
};

char* vCjson::get_node_value_str()
{
    if (!_curr_node) return nullptr;

    if (!_curr_node->value_len) return nullptr;

    wipe_value_str();

    _value_str = new char[_curr_node->value_len + 1];
    strncpy(_value_str, _curr_node->value, _curr_node->value_len);
    _value_str[_curr_node->value_len] = 0;
    return _value_str;
};

char* vCjson::get_node_name_str()
{
    if (!_curr_node) return nullptr;

    if (!_curr_node->name_len) return nullptr;

    wipe_name_str();

    _name_str = new char[_curr_node->name_len + 1];
    strncpy(_name_str, _curr_node->name, _curr_node->name_len);
    _name_str[_curr_node->name_len] = 0;
    return _name_str;
};

char* vCjson::cp_node_name(char* buff)
{
    if (_qnt_nodes)
    {
        if (_curr_node)
        {
            if (buff)
            {
                strncpy(buff, _curr_node->name, _curr_node->name_len);
                buff[_curr_node->name_len] = 0;
                return buff;
            }
        }
    }
    return nullptr;
};

char* vCjson::cp_node_value(char* buff)
{
    if (_qnt_nodes)
    {
        if (_curr_node)
        {
            if (buff)
            {
                strncpy(buff, _curr_node->value, _curr_node->value_len);
                buff[_curr_node->value_len] = 0;
                return buff;
            }
        }
    }
    return nullptr;
};

int vCjson::parse(char* jsonData)
{
    /*CLEAR OLD DATA*/
    wipe_all();

    /*PREP DATA*/
    int res;
    res = prep_data(jsonData);
    if (res != VCJSON_OK)
    {
        wipe_all();
        return res;
    }

    /*PARSE QOUTES IN JSON DATA*/
    _qt = new Quotes;
    if (_qt->parse((const char*)_buff))
    {
        wipe_all();
        return VCJSON_ERR_INCORRECT_DATA_QT;
    }

    /*PARSE BRACESES IN JSON DATA*/
    _br = new Braces;
    if (_br->parse(_buff, "[{", "]}", true))
    {
        wipe_all();
        return VCJSON_ERR_INCORRECT_DATA_BR;
    }

    _br->order_asc();
    _br->deep_lvl();


    /*PARSE COMMAS IN JSON DATA*/
    bool skip;
    int pos_comma;
    int comma_n = 0;
    int qnt_commas_all = 0;
    int qnt_commas_curr_lvl;
    int deep_lvl_for_comma;


    pos_comma = fndchr(_buff, ',', strlen(_buff), 0);
    while (pos_comma >= 0)
    {
        skip = false;
        for (int i = 0; i < _qt->get_len(); i++)
        {
            if ((pos_comma > _qt->arr_open[i]) && (pos_comma < _qt->arr_close[i]))
            {
                skip = true;
                break;
            }
        }
        if (!skip)  qnt_commas_all++;

        pos_comma = fndchr((char*)_buff, ',', strlen(_buff), pos_comma + 1);
    }

    _arr_commas_pos = new int[qnt_commas_all];
    _arr_commas_deep = new int[qnt_commas_all];

    for (int curr_lvl = 0; curr_lvl <= _br->get_deep_lvl(); curr_lvl++)
    {
        qnt_commas_curr_lvl = 0;

        pos_comma = fndchr(_buff, ',', strlen(_buff), 0);
        while (pos_comma >= 0)
        {
            skip = false;
            for (int i = 0; i < _qt->get_len(); i++)
            {
                if ((pos_comma > _qt->arr_open[i]) && (pos_comma < _qt->arr_close[i]))
                {
                    skip = true;
                    break;
                }
            }
            if (!skip)
            {
                deep_lvl_for_comma = 0;
                for (int b = 0; b < _br->get_len(); b++)
                {
                    if ((pos_comma > _br->arr_open[b]) && (pos_comma < _br->arr_close[b]))
                    {
                        if (deep_lvl_for_comma < _br->arr_deep_lvl[b]) deep_lvl_for_comma = _br->arr_deep_lvl[b];
                    }
                }

                if (deep_lvl_for_comma == curr_lvl)
                {
                    _arr_commas_pos[comma_n] = pos_comma;
                    _arr_commas_deep[comma_n] = curr_lvl;
                    comma_n++;
                    qnt_commas_curr_lvl++;
                }
            }
            pos_comma = fndchr(_buff, ',', strlen(_buff), pos_comma + 1);
        }
    }

    /*PARSE NODES IN JSON DATA*/

    int curr_node;
    int commas_in_interval;
    char chr;
    int colon_pos;

    for (int i = 0; i < _br->get_len(); i++)
    {
        commas_in_interval = 0;

        for (int n = 0; n < qnt_commas_all; n++)
        {
            if ((_arr_commas_pos[n] > _br->arr_open[i]) && (_arr_commas_pos[n] < _br->arr_close[i]) && (_arr_commas_deep[n] == _br->arr_deep_lvl[i]))
            {
                commas_in_interval++;
            }
        }
        if (commas_in_interval > 0)
        {
            _qnt_nodes =  _qnt_nodes + commas_in_interval + 1;
        }
        else
        {
            if (_br->arr_close[i] == _br->arr_open[i] + 1)
            {
            }
            else
            {
                _qnt_nodes = _qnt_nodes + 1;
            }
        }
    }

    if (_qnt_nodes == 0)
    {
        wipe_all();
        return VCJSON_ERR_EMPTY_JSON_DATA;
    }

    _arr_nodes = new st_node[_qnt_nodes];

    curr_node = 0;
    for (int i = 0; i < _br->get_len(); i++)
    {
        if (_br->arr_close[i] == _br->arr_open[i] + 1) continue;

        _arr_nodes[curr_node].pos_begin = _br->arr_open[i] + 1;

        for (int n = 0; n < qnt_commas_all; n++)
        {
            if ((_arr_commas_pos[n] > _br->arr_open[i]) && (_arr_commas_pos[n] < _br->arr_close[i]) && (_arr_commas_deep[n] == _br->arr_deep_lvl[i]))
            {
                _arr_nodes[curr_node].pos_end = _arr_commas_pos[n] - 1;
                curr_node++;
                _arr_nodes[curr_node].pos_begin = _arr_commas_pos[n] + 1;
            }
        }

        _arr_nodes[curr_node].pos_end = _br->arr_close[i] -1;
        curr_node++;
    }



    for (int n = 0; n < _qnt_nodes; n++)
    {
        chr = *(_buff + _arr_nodes[n].pos_begin);


        if (chr == '"')
        {
            colon_pos = fndchr(_buff, ':', strlen(_buff), _arr_nodes[n].pos_begin);

            if ((colon_pos < _arr_nodes[n].pos_end) && (colon_pos != -1))
            {
                chr = *(_buff + colon_pos + 1);
                if (get_type_of_node(&_arr_nodes[n], chr))
                {
                    wipe_all();
                    return VCJSON_ERR_INCORRECT_VALUE;
                }
                if (_arr_nodes[n].type == VCJSON_NT_STR) _arr_nodes[n].type = VCJSON_NT_PNV;

                _arr_nodes[n].name = (_buff + _arr_nodes[n].pos_begin + 1);
                for (int q = 0; q < _qt->get_len(); q++)
                {
                    if (_arr_nodes[n].pos_begin == _qt->arr_open[q])
                    {
                        _arr_nodes[n].name_len = _qt->arr_close[q] - _qt->arr_open[q] - 1;
                        break;
                    }
                }

                if ((_arr_nodes[n].type == VCJSON_NT_OBJ) || (_arr_nodes[n].type == VCJSON_NT_ARR) || (_arr_nodes[n].type == VCJSON_NT_PNV))
                {
                    _arr_nodes[n].value = (_buff + colon_pos + 2);
                    _arr_nodes[n].value_len = _arr_nodes[n].pos_end - colon_pos - 2;
                }
                else
                {
                    _arr_nodes[n].value = (_buff + colon_pos + 1);
                    _arr_nodes[n].value_len = _arr_nodes[n].pos_end - colon_pos;
                }
            }
            else
            {
                _arr_nodes[n].type = VCJSON_NT_STR;
                _arr_nodes[n].value = (_buff + _arr_nodes[n].pos_begin + 1);
                _arr_nodes[n].value_len = _arr_nodes[n].pos_end - _arr_nodes[n].pos_begin - 1;

            }
        }
        else
        {
            if (get_type_of_node(&_arr_nodes[n], chr))
            {
                wipe_all();
                return VCJSON_ERR_INCORRECT_VALUE;
            }

            if ((_arr_nodes[n].type == VCJSON_NT_OBJ) || (_arr_nodes[n].type == VCJSON_NT_ARR))
            {
                _arr_nodes[n].value = (_buff + _arr_nodes[n].pos_begin + 1);
                _arr_nodes[n].value_len = _arr_nodes[n].pos_end - _arr_nodes[n].pos_begin - 1;
            }
            else
            {
                _arr_nodes[n].value = (_buff + _arr_nodes[n].pos_begin);
                _arr_nodes[n].value_len = _arr_nodes[n].pos_end - _arr_nodes[n].pos_begin + 1;
            }
        }
    }


    for (int n = 0; n < _qnt_nodes; n++)
    {
        _arr_nodes[n].parent_id = 0;
        _arr_nodes[n].self_id = n + 1;
    }

    for (int n = 0; n < _qnt_nodes; n++)
    {
        for (int m = 0; m < _qnt_nodes; m++)
        {
            if ((_arr_nodes[n].pos_begin > _arr_nodes[m].pos_begin) &&  (_arr_nodes[n].pos_end < _arr_nodes[m].pos_end))
                if (_arr_nodes[n].parent_id < _arr_nodes[m].self_id)
                    _arr_nodes[n].parent_id = _arr_nodes[m].self_id;
        }
    }

    /*CLEAN MEMORY*/
    wipe_temp();

    return VCJSON_OK;
};

void vCjson::show_node_info()
{
    printf("\n\nNODE: %i\n", _curr_node->self_id);
    char* nameBuf = new char[_curr_node->name_len +1];
    char* valBuf = new char[_curr_node->value_len +1];


    strncpy(nameBuf, _curr_node->name, _curr_node->name_len);
    nameBuf [_curr_node->name_len] = 0;

    strncpy(valBuf, _curr_node->value, _curr_node->value_len);
    valBuf [_curr_node->value_len] = 0;

    printf("Type: ");

    switch (_curr_node->type)
    {
    case VCJSON_NT_ARR :
        printf("ARR");
        break;
    case VCJSON_NT_FALSE :
        printf("FLS");
        break;
    case VCJSON_NT_NUL :
        printf("NUL");
        break;
    case VCJSON_NT_NUM :
        printf("NUM");
        break;
    case VCJSON_NT_OBJ :
        printf("OBJ");
        break;
    case VCJSON_NT_PNV :
        printf("PNV");
        break;
    case VCJSON_NT_STR :
        printf("STR");
        break;
    case VCJSON_NT_TRUE :
        printf("TRU");
        break;
    }
    printf(" begin:%i end:%i parentID:%i selfID:%i\n", _curr_node->pos_begin, _curr_node->pos_end, _curr_node->parent_id, _curr_node->self_id);
    printf("Name len:%i Value len:%i\n", _curr_node->name_len, _curr_node->value_len);
    printf("Name:\"%s\"\nValue:\n%s\n", nameBuf, valBuf);

    delete []nameBuf;
    delete []valBuf;
};

void vCjson::show_node()
{
    char* nameBuf = new char[_curr_node->name_len +1];
    char* valBuf = new char[_curr_node->value_len +1];

    strncpy(nameBuf, _curr_node->name, _curr_node->name_len);
    nameBuf [_curr_node->name_len] = 0;

    strncpy(valBuf, _curr_node->value, _curr_node->value_len);
    valBuf [_curr_node->value_len] = 0;

    switch (_curr_node->type)
    {
    case VCJSON_NT_ARR :
        printf("ARR");
        break;
    case VCJSON_NT_FALSE :
        printf("FLS");
        break;
    case VCJSON_NT_NUL :
        printf("NUL");
        break;
    case VCJSON_NT_NUM :
        printf("NUM");
        break;
    case VCJSON_NT_OBJ :
        printf("OBJ");
        break;
    case VCJSON_NT_PNV :
        printf("PNV");
        break;
    case VCJSON_NT_STR :
        printf("STR");
        break;
    case VCJSON_NT_TRUE :
        printf("TRU");
        break;
    }

    printf(" \"%s\" : %s\n", nameBuf, valBuf);

    delete []nameBuf;
    delete []valBuf;
};

int vCjson::prep_data (char* jsonData)
{
    if (!jsonData) return VCJSON_ERR_BUFF_IS_EMPTY;

    Quotes qt = Quotes();

    if (qt.parse((const char*)jsonData))
    {
        return VCJSON_ERR_INCORRECT_DATA_QT;
    }

    _buff = new char [strlen(jsonData) + 1];
    memset (_buff, 0x00, strlen(jsonData) + 1);

    bool quotes_hit;
    int begin_pos = 0;
    int buff_offset= 0;
    int len = 0;
    for (int c_pos = 0; (unsigned)c_pos < strlen(jsonData); c_pos++)
    {
        for (int s = 0; s < _symb_num; s++)
        {
            if (jsonData[c_pos] == _symb_avoid[s])
            {
                if (_symb_avoid[s] == 0x20)
                {
                    quotes_hit = false;

                    for (int q = 0; q < qt.get_len(); q++)
                    {
                        if ((c_pos > qt.arr_open[q]) && (c_pos < qt.arr_close[q]))
                        {
                            quotes_hit = true;
                            break;
                        }
                    }

                    if (quotes_hit) break;
                }
                len = c_pos - begin_pos;
                memcpy(_buff + buff_offset, (jsonData + begin_pos), len);
                begin_pos = c_pos + 1;
                buff_offset += len;
            }
        }
    }
    memcpy(_buff + buff_offset, (jsonData + begin_pos), strlen(jsonData) - begin_pos);

    return VCJSON_OK;
};

int vCjson::get_type_of_node(st_node * node, char chr)
{
    switch (chr)
    {
    case '"' :
        node->type = VCJSON_NT_STR;
        break;
    case '[' :
        node->type = VCJSON_NT_ARR;
        break;
    case '{' :
        node->type = VCJSON_NT_OBJ;
        break;
    case 't' :
        node->type = VCJSON_NT_TRUE;
        break;
    case 'f' :
        node->type = VCJSON_NT_FALSE;
        break;
    case 'n' :
        node->type = VCJSON_NT_NUL;
        break;
    case '-' :
    case '0' :
    case '1' :
    case '2' :
    case '3' :
    case '4' :
    case '5' :
    case '6' :
    case '7' :
    case '8' :
    case '9' :
        node->type = VCJSON_NT_NUM;
        break;

    default:
        return -1;
        break;
    }
    return 0;
}

void vCjson::wipe_name_str()
{
    if (_name_str)
    {
        delete []_name_str;
        _name_str = nullptr;
    }
};

void vCjson::wipe_value_str()
{
    if (_value_str)
    {
        delete []_value_str;
        _value_str = nullptr;
    }
};

void vCjson::wipe_temp()
{
    if (_arr_commas_pos)
    {
        delete []_arr_commas_pos;
        _arr_commas_pos = nullptr;
    }
    if (_arr_commas_deep)
    {
        delete []_arr_commas_deep;
        _arr_commas_deep = nullptr;
    }
    if (_qt)
    {
        _qt->wipe();
        delete _qt;
        _qt = nullptr;
    }
    if (_br)
    {
        _br->wipe();
        delete _br;
        _br = nullptr;
    }
};

void vCjson::wipe_all()
{
    if (_buff != nullptr)
    {
        delete []_buff;
        _buff = nullptr;
    }

    wipe_temp();
    wipe_name_str();
    wipe_value_str();

    if (_arr_nodes)
    {
        delete []_arr_nodes;
        _arr_nodes = nullptr;
        _qnt_nodes = 0;
    }

    _curr_node = nullptr;
    _curr_parent_id = 0;
    _curr_self_id = 0;
};

int vCjson::fndchr(char * str, char chr, int len, int offset)
{
    if (str)
    {
        for (int i = 0; i < len; i++)
        {
            if (str[i + offset] == chr) return (i + offset);
        }
    }
    return -1;
}
