#include "parson.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* get_lib_name()
{
    return "parson";
}

void* parse_json(const char* json, size_t length)
{
    JSON_Value* doc = json_parse_string_with_comments(json);
    return doc;
}

const char* stringify_json(void* _doc, size_t* string_length)
{
    JSON_Value* doc = (JSON_Value*)_doc;
    const char* s = json_serialize_to_string(doc);
    *string_length = strlen(s);
    return s;
}

void free_json(void* doc)
{
    json_value_free((JSON_Value*)doc);
}

void traverse_json(void* _doc, double* sum)
{
}