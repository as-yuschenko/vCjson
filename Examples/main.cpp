#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "../vCjson.h"




///Recursive function to output a tree of the JSON data
int showTree (vCjson* json, int lvl = 0);


int main()
{
    int res, fd, len;
    char data[10000];

    //try to open file
    fd = open("test.json", O_RDONLY);
    if (fd < 0)
    {
        printf("Err open file\n");
        return -1;
    }

    //try to read from file
    len = read (fd, data, 9999);
    if (len < 1)
    {
        printf("Err read from file\n");
        close(fd);
        return -1;
    }
    data[len] = 0x00;

    //Create an instance of the class
    vCjson json = vCjson();

    // first parse JSON data
    res = json.parse(data);
    close(fd);

    if (res != VCJSON_OK)
    {
        printf("Parse err: %i\n", res); //err codes defined in vCjson.h
        return -1;
    }
    //Next we can output to console tree of JSON data
    showTree (&json);

    //Lets return cursor to root
    json.go_root();




    //Lets output to console e-mails of each entity
    printf("\nE-mail addresses:\n");

    while (!json.go_next_sibling())
    {
        if (!json.go_node_child())
        {
            if (!json.go_node_name_on_layer("email"))
            {
                printf("\t%s\n", json.get_node_value_str());
            }
            json.go_node_parent();
        }
    }

    //Lets return cursor to root
    json.go_root();




    //Lets output to console name of each entity if it has tag "ipsum"
    printf("\nNames:\n");
    char tag[] = "ipsum";
    char buff[200]; //buff to store id
    while (!json.go_next_sibling())
    {
        if (!json.go_node_child())
        {
            //Find "name"
            if (!json.go_node_name_on_layer("name"))
            {
                json.cp_node_value(buff); // store name to buff;

                //Find "tags"
                if (!json.go_node_name_on_layer("tags"))
                {
                    if (!json.go_node_child())
                    {
                        while (!json.go_next_sibling())
                        {
                            if (!strncmp(json.get_node_value_ptr(), tag, strlen(tag))) //compare
                            {
                                printf("\t%s\n", buff);
                                break;
                            }
                        }
                        json.go_node_parent();
                    }
                }
            }
            json.go_node_parent();
        }
    }

    json.~vCjson();

    return 0;
}
int showTree(vCjson* json, int lvl)
{
    while (1)
    {
        if(json->go_next_sibling()) break;
        for (int i = 0; i < lvl; i++) printf("\t");
        json->show_node();

        while (1)
        {
            if(json->go_node_child()) break;
            showTree (json, lvl + 1);
            json->go_node_parent();
            break;
        }
    }
    return 0;
}
