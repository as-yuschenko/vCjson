# vCjson
## Parsing
After parsing the JSON document, JSON data is split into nodes. Each node is a data type according to https://www.json.org/ and can be an object, array, string, number, key-value pair, true, false, null.
All nodes are arranged hierarchically according to the nesting depth.<br />
To parse a JSON document, you must use the method:
>int parse(char* jsonData);

Attention! The cursor takes the root position and not point to any node of the current level.
## Nodes info
Each node has the following properties:<br />
+ Type (object, array, string, number, key-value pair, true, false, null);
+ Own ID;
+ Parent ID;
+ Start position of node data;
+ End position of node data;
+ Pointer to node name (if exists);
+ Size of the node name (if exists);
+ Pointer to node value (if exists);
+ The size of the value (if exists);

To get the number of nodes in a JSON document, use the method:<br />
>int get_num_nodes();

To get type of current node, use method:
> int get_node_type();

To get type of node that name was specified, use method:
> int get_node_type_by_name(const char* name);


## Navigate
The library implements two mechanisms for moving through nodes:
+ Cursor;
+ By node name (if it exists).
  
### Navigate with the cursor
After parsing the JSON document, the cursor is in the root position and does not point to any node.<br />
To get the number of nodes at the current level, use the method:
> int get_sibling_num();

To move between nodes of the same level, use following methods:
> int go_next_sibling();<br />
> int go_prev_sibling();

A node can be a parent of other nodes. To go to the child nodes of the current node (go down one level), use the method:
> int go_node_child();

Attention! The cursor takes the root position and not point to any node of the current level.<br />
To navigate through nodes, also use the go_next_sibling, go_prev_sibling, and get_sibling_num methods to get the number of nodes at this level. If there is node at this level, that is a parent to other nodes, use the same navigation mechanism.<br />

To return to the parent node (the cursor will point to it), use the method:
> int go_node_parent();

To return the cursor to the top-level root position, use the method:
> void go_root();

### Navigate by node name
To navigate by node name, use the method:
> int go_node_name (const char* name);

It sets the cursor to the first node whose name is passed by the pointer. The search is performed starting from the root position of the document.<br />
  
If the JSON document has multiple nodes with the same name at different nesting levels, you can move the cursor to the position of the parent node using go_node_name, go_next_sibling, go_prev_sibling, go_node_child, or go_node_parent, then move cursor to the child nodes of that node using go_node_child, and use the method:
> int go_node_name_on_layer(const char* name);

It sets the cursor to the node passed by the pointer at the current nesting level.

## Data extraction
To get nodes names and their data value, use the following methods:
> char* get_node_name_ptr();<br />
> int get_node_name_len();<br />
> char* get_node_value_ptr();<br />
> int get_node_value_len();


The following methods can be used for comparison of node names or node data values when making decisions.
> char* get_node_value_str();<br />
> char* get_node_name_str();

Be careful, each time you use the methods, the buffers will contain information about the values of the name and data of the current node.

To copy node name or node data value, use methods: 
> char* cp_node_name(char* buff);<br />
> char* cp_node_value(char* buff);

Be careful, the buffer size must be at least the data size + 1 byte. First use get_node_name_len or get_node_value_len to determine the size of the buffer.
