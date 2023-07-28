# vCjson
## Parsing
After parsing the JSON document, JSON data is split into nodes. Each node is a data type according to https://www.json.org/ and can be an object, array, string, number, key-value pair, true, false, null.
All nodes are arranged hierarchically according to the nesting depth.<br />
To parse a JSON document, you must use the method:
>int parse(char* jsonData);

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

## Moving
The library implements two mechanisms for moving through nodes:
+ Cursor;
+ By node name (if it exists).
  
### Moving with the cursor
After parsing the JSON document, the cursor is in the root position and does not point to any node.<br />
To get the number of nodes at the current level, you must use the method:
> int get_sibling_num();
To move between nodes of the same level, use following methods:
> int go_next_sibling();<br />
> int go_prev_sibling();


In order to find out the number of nodes at the current level, you must use the method
get_sibling_num returns the number of nodes in the current level.


A node can be a parent of other nodes within it. In order to go to the child nodes of the current node (go down one level), you must use the method:
go_node_child moves the cursor to the child level of the current node.
Be careful, the cursor will take the root position and will not point to any node of the current level.

To navigate through nodes, also use the go_next_sibling, go_prev_sibling, and get_sibling_num methods to determine the number of nodes at that level. If there is a parent node at this level, then use the same navigation mechanism.

To return to the parent node (the cursor will point to it), use the method
go_node_parent moves the cursor to the parent element.

To return the cursor to the top-level root position, use the method
go_root moves the cursor to the root position of the JSON document.
