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
#ifndef VCJSON_H_INCLUDED
#define VCJSON_H_INCLUDED

#include "Quotes.h"
#include "Braces.h"


/*OK*/
#define     VCJSON_OK         0

/*TYPES OF NODES*/
#define     VCJSON_NT_STR     1 //string
#define     VCJSON_NT_NUM     2 //numder
#define     VCJSON_NT_OBJ     3 //object
#define     VCJSON_NT_ARR     4 //array
#define     VCJSON_NT_PNV     5 //pair key-value
#define     VCJSON_NT_TRUE    6 //true
#define     VCJSON_NT_FALSE   7 //false
#define     VCJSON_NT_NUL     8 //null


/*PARSING STATUS_CODES:*/
#define     VCJSON_ERR_EMPTY_JSON_DATA     100
#define     VCJSON_ERR_INCORRECT_DATA_QT   101
#define     VCJSON_ERR_INCORRECT_DATA_BR   102
#define     VCJSON_ERR_INCORRECT_VALUE     103
#define     VCJSON_ERR_BUFF_IS_EMPTY       104

class vCjson
{
public:

    vCjson();
    ~vCjson();

    /** Makes parsing of json data.
    * ATTENTION!!!
    * When data parsing complete, cursor located on root position.
    * Use go_next_sibling() to move to the first sibling.
    *
    * @param str - pointer to zero terminated c-string.
    * @return VCJSON_OK or  VCJSON_ERR_EMPTY_JSON_DATA, or
                            VCJSON_ERR_INCORRECT_DATA_QT, or
                            VCJSON_ERR_INCORRECT_DATA_BR, or
                            VCJSON_ERR_INCORRECT_VALUE, or
                            VCJSON_ERR_BUFF_IS_EMPTY.
    */
    int parse(char* jsonData);


    /*NODES INFO*/
    /** Returns number of nodes.
    * @return number of nodes
    */
    int get_num_nodes();

    /** Returns type of current node.
    * @return
    * VCJSON_NT_STR,
    * VCJSON_NT_NUM,
    * VCJSON_NT_OBJ,
    * VCJSON_NT_ARR,
    * VCJSON_NT_PNV,
    * VCJSON_NT_TRUE,
    * VCJSON_NT_FALSE,
    * VCJSON_NT_NUL
    */
    int get_node_type();

    /** Returns type of node that name specified.
    * @return
    * VCJSON_NT_STR,
    * VCJSON_NT_NUM,
    * VCJSON_NT_OBJ,
    * VCJSON_NT_ARR,
    * VCJSON_NT_PNV,
    * VCJSON_NT_TRUE,
    * VCJSON_NT_FALSE,
    * VCJSON_NT_NUL
    * or [-1] if error.
    */
    int get_node_type_by_name(const char* name);


    /*MOVING BY NODE NAME*/
    /** Moves cursor position to node that name specified.
    * @param name - pointer to zero terminated c-string.
    * @return [0] - success, [-1] - error.
    */
    int go_node_name (const char* name);

    /** Moves cursor position to node that name specified if node exists on current level.
    * @param name - pointer to zero terminated c-string.
    * @return [0] - success, [-1] - error.
    */
    int go_node_name_on_layer(const char* name);




    /*MOVING BY CURSOR*/
    /** Returns number of siblings.
    * @return number of siblings on current layer.
    */
    int get_sibling_num();

    /** Moves cursor to next (or first) sibling of current layer.
    * @return [0] - success, [-1] - current node is the last or error.
    */
    int go_next_sibling();

    /** Moves cursor to previos sibling of current layer.
    * @return [0] - success, [-1] - current node is the first or error.
    */
    int go_prev_sibling();

    /** Moves cursor to childs of the current node.
    * ATTENTION!!!
    * Cursor located on current layer root position.
    * @use go_next_sibling() to move to the first sibling.
    * @use get_sibling_num() to know how much siblings.
    * @return [0] - success, [-1] - no childs for current node or error.
    */
    int go_node_child();

    /** Moves cursor to parent of current node.
    * @return [0] - success, [-1] - parent of current node is root or error.
    */
    int go_node_parent();

    /** Moves cursor to the root.*/
    void go_root();


    /*DATA EXTRACTION*/
    /** Returns pointer to name of current node.
    * To get len of the name @use get_node_name_len().
    * @return pointer or nullptr if current node have no name.
    */
    char* get_node_name_ptr();

    /** Returns len of the name of current node.
    * To get pointer of the name @use get_node_name_ptr().
    * @return len or 0 if current node have no name.
    */
    int get_node_name_len();

    /** Returns pointer to the value of current node.
    * To get len of the value @use get_node_value_len().
    * @return pointer or nullptr if current node have no value.
    */
    char* get_node_value_ptr();

    /** Returns len of the value of current node.
    * To get pointer of the value @use get_node_value_ptr().
    * @return len  or 0 if current node has no value.
    */
    int get_node_value_len();

    /** Returns pointer to buff as zero terminated c-string contains value of current node.
    * @return pointer or nullptr if current node has no value.
    */
    char* get_node_value_str();

    /** Returns pointer to buff as zero terminated c-string contains name of current node.
    * @return pointer or nullptr if current node have no name.
    */
    char* get_node_name_str();

    /** Copy to buff name of current node.
    * @param buff - pointer to buff. Buffer size must be at least data size + 1 byte,
    * @use get_node_name_len() to get name len.
    * @return pointer or nullptr if error.
    */
    char* cp_node_name(char* buff);

    /** Copy to buff value of current node.
    * @param buff - pointer to buff. Buffer size must be at least data size + 1 byte,
    * @use get_node_value_len() to get value len.
    * @return pointer or nullptr if error.
    */
    char* cp_node_value(char* buff);

    /*SHOW NODE INFO*/
    /**Prints to console verbose information about node*/
    void show_node_info();

    /**Prints to console lite information about node*/
    void show_node();


private:

    struct st_node
    {
        int type;
        int pos_begin;
        int pos_end;

        int self_id;
        int parent_id;

        char * name = nullptr;
        int name_len = 0;

        char * value = nullptr;
        int value_len = 0;
    };


    st_node* _curr_node = nullptr;
    int _curr_parent_id = 0;
    int _curr_self_id = 0;


    int _qnt_nodes = 0;
    st_node* _arr_nodes = nullptr;


    int get_type_of_node(st_node * node, char chr);

    Braces* _br = nullptr;
    Quotes* _qt = nullptr;


    int * _arr_commas_pos = nullptr;
    int * _arr_commas_deep = nullptr;



    char* _name_str = nullptr;
    char* _value_str = nullptr;

    void wipe_name_str();
    void wipe_value_str();

    char* _buff = nullptr;

    static const int _symb_num = 4;
    char _symb_avoid[_symb_num] = {0x20, 0x0A, 0x09, 0x0D};

    int prep_data (char* jsonData);

    void wipe_temp();

    void wipe_all();

    int fndchr(char * str, char chr, int len, int offset);

};

#endif // VCJSON_H_INCLUDED
