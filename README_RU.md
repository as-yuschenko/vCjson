# vCjson
## Назначение
**Маленькая библиотека, разработанная для парсинга структуры JSON документа и легкого извлечения данных, написанная на c++**
## Парсинг
Во время парсинга структуры документа JSON данные разбиваются на узлы. Каждый узел представляет собой тип данных в соответствии с https://www.json.org/ и может являться объектом, массивом, строкой, числом, парой ключ - значение, TRUE, FALSE, NULL.
Все узлы выстраиваются иерархически, в соответствии с глубиной вложенности. <br>
Для парсинга документа JSON используется метод:
```cpp
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
```
Внимание! После парсинга курсор занимает позицию root и не указывает ни на один узел.

## Информация о узлах
Каждый узел обладает следующими свойствами:

+ Тип (объект, массив, строка, число, пара ключ - значение, правда, ложь, нуль);
+ Собственный ID;
+ ID Родителя;
+ Позиция начала данных узла;
+ Позиция окончания данных узла;
+ Указатель на имя узла (если существует);
+ Размер имени узла (если существует);
+ Указатель на значение узла (если существует);
+ Размер значения узла (если существует);

Для получения количества узлов документа JSON используется метод:<br />
```cpp
    /** Returns number of nodes.
    * @return number of nodes
    */
    int get_num_nodes();
```
Для получения типа текущего узла используется метод:
```cpp
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
```
Для получения типа узла, переданного по имени используется метод:
```cpp
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
```

## Навигация
В библиотеке реализовано два способа навигации по документу:
+ Курсор;
+ По имени узла (если существует).
  
### Навигация с помощью курсора
После парсинга курсор занимает позицию root и не указывает ни на один узел.<br />
Для получения количества узлов на текущем уровне вложенности используется метод:
```cpp
    /** Returns number of siblings.
    * @return number of siblings on current layer.
    */
    int get_sibling_num();
```
Для перемещения между узлами на текущем уровне вложенности используются методы:
```cpp
    /** Moves cursor to next (or first) sibling of current layer.
    * @return [0] - success, [-1] - current node is the last or error.
    */
    int go_next_sibling();

    /** Moves cursor to previos sibling of current layer.
    * @return [0] - success, [-1] - current node is the first or error.
    */
    int go_prev_sibling();
```

Узел может быть родительским по отношению к другим узлам, входящим в его состав. Для того, чтобы перейти к дочерним узлам текущего узла (опуститься на уровень ниже) используется метод:
```cpp
    /** Moves cursor to childs of the current node.
    * ATTENTION!!!
    * Cursor located on current layer root position.
    * @use go_next_sibling() to move to the first sibling.
    * @use get_sibling_num() to know how much siblings.
    * @return [0] - success, [-1] - no childs for current node or error.
    */
    int go_node_child();
```
После применения метода курсор занимает позицию root текущего уровня и не указывает ни на один узел.<br />
Для перемещения по узлам текущего уровня используйте методы go_next_sibling, go_prev_sibling и get_sibling_num для определения количества узлов на этом уровне. Если на текущем уровне есть узел, являющийся родительским по отношению к другим узлам, то используйте тот же механизм перемещения. <br />

Для возврата к родительскому узлу (курсор будет указывать на него) используется метод:
```cpp
    /** Moves cursor to parent of current node.
    * @return [0] - success, [-1] - parent of current node is root or error.
    */
    int go_node_parent();
```
Для возврата курсора в root положение самого верхнего уровня используется метод:
```cpp
    /** Moves cursor to the root.*/
    void go_root();
```
### Навигация по имени узла
Для перемещения курсора к узлу, имя которого было передано, используется метод:
```cpp
    /** Moves cursor position to node that name specified.
    * @param name - pointer to zero terminated c-string.
    * @return [0] - success, [-1] - error.
    */
    int go_node_name (const char* name);
```
Метод устанавливает курсор на первый по порядку узел имя которого передано по указателю. Поиск производится начиная с позиции root документа.<br />
  
If the JSON document has multiple nodes with the same name at different nesting levels, you can move the cursor to the position of the parent node using go_node_name, go_next_sibling, go_prev_sibling, go_node_child, or go_node_parent, then move cursor to the child nodes of that node using go_node_child, and use the method:
```cpp
    /** Moves cursor position to node that name specified if node exists on current level.
    * @param name - pointer to zero terminated c-string.
    * @return [0] - success, [-1] - error.
    */
    int go_node_name_on_layer(const char* name);
```
It sets the cursor to the node passed by the pointer at the current nesting level.

## Data extraction
To get nodes names and their data value, use the following methods:
```cpp
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
```

The following methods can be used for comparison of node names or node data values when making decisions.
```cpp
    /** Returns pointer to buff as zero terminated c-string contains value of current node.
    * @return pointer or nullptr if current node has no value.
    */
    char* get_node_value_str();

    /** Returns pointer to buff as zero terminated c-string contains name of current node.
    * @return pointer or nullptr if current node have no name.
    */
    char* get_node_name_str();
```

Be careful, each time you use the methods, the buffers will contain information about the values of the name and data of the current node.

To copy node name or node data value, use methods: 
```cpp
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
```

Be careful, the buffer size must be at least the data size + 1 byte. First use get_node_name_len or get_node_value_len to determine the size of the buffer.

## Show info

To show information about node, use methods:
```cpp
    /**Prints to console verbose information about node*/
    void show_node_info();

    /**Prints to console lite information about node*/
    void show_node();
```

