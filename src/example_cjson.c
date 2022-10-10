//
// clang -o test2 -DYYJSON_DISABLE_WRITER -DYYJSON_DISABLE_FAST_FP_CONV=1 example.c src/yyjson.c -Os -g
// https://github.com/ibireme/yyjson/blob/master/doc/API.md

#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>

const char* get_lib_name()
{
    return "cjson";
}

void* parse_json(const char* json, size_t length, double* sum)
{
    cJSON* doc = cJSON_ParseWithLength(json, length);
    return doc;
}

const char* stringify_json(void* _doc, size_t* string_length)
{
    cJSON* doc = (cJSON*)_doc;
    return cJSON_PrintUnformatted(doc);
}

void free_json(void* doc)
{
    cJSON_Delete((cJSON*)doc);
}

// #define cJSON_Invalid (0)
// #define cJSON_False  (1 << 0)
// #define cJSON_True   (1 << 1)
// #define cJSON_NULL   (1 << 2)
// #define cJSON_Number (1 << 3)
// #define cJSON_String (1 << 4)
// #define cJSON_Array  (1 << 5)
// #define cJSON_Object (1 << 6)
// #define cJSON_Raw    (1 << 7) /* raw json */


    // /* next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem */
    // struct cJSON *next;
    // struct cJSON *prev;
    // /* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */
    // struct cJSON *child;

    // /* The type of the item, as above. */
    // int type;

    // /* The item's string, if type==cJSON_String  and type == cJSON_Raw */
    // char *valuestring;
    // /* writing to valueint is DEPRECATED, use cJSON_SetNumberValue instead */
    // int valueint;
    // /* The item's number, if type==cJSON_Number */
    // double valuedouble;


static void traverse_node(cJSON* node, double* ctx)
{
    int type = node->type;
    if (type == cJSON_Array || type == cJSON_Object)
    {
        cJSON* item = node->child;
        while(item != NULL)
        {
            traverse_node(item, ctx);
            item = item->next;
        }
    }
    else if (type == cJSON_Number)
    {
        *ctx += node->valuedouble;
    }
    else if (type == cJSON_True)
    {
        *ctx += 1;
    }
}

void traverse_json(void* _doc, double* sum)
{
    cJSON* doc = (cJSON*)_doc;
    traverse_node(doc, sum);
}