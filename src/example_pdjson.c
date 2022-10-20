//
// clang -o test2 -DYYJSON_DISABLE_WRITER -DYYJSON_DISABLE_FAST_FP_CONV=1 example.c src/yyjson.c -Os -g
// https://github.com/ibireme/yyjson/blob/master/doc/API.md

#include "pdjson.h"
#include <stdio.h>
#include <stdlib.h>

const char* get_lib_name()
{
    return "pdjson";
}

void* parse_json(const char* buffer, size_t length, double* sum)
{
    struct json_stream json[1];

    json_open_buffer(json, buffer, length);
    //json_set_streaming(json, false);

    int success = 1;
    enum json_type type = json_next(json);
    while (type > JSON_DONE)
    {
        switch(type)
        {
        case JSON_STRING:
            {
                size_t strlength = 0;
                const char* str = json_get_string(json, &strlength);
            }
            break;
        case JSON_NUMBER:
            *sum += json_get_number(json);
            break;
        case JSON_TRUE:
            *sum += 1;
            break;
        default:
            break;
        }

        type = json_next(json);
    }

    if (!success) {
        printf("centijson: %s\n", json_get_error(json));
        json_close(json);
        return 0;
    }
    json_close(json);
    return malloc(1); // to make it think we have a document
}

const char* stringify_json(void* _doc, size_t* string_length)
{
    return 0;
}

void free_json(void* doc)
{
    free(doc);
}

void traverse_json(void* _doc, double* sum)
{
    *sum = 0;
}