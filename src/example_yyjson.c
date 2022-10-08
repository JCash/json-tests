//
// clang -o test2 -DYYJSON_DISABLE_WRITER -DYYJSON_DISABLE_FAST_FP_CONV=1 example.c src/yyjson.c -Os -g
// https://github.com/ibireme/yyjson/blob/master/doc/API.md

#include "src/yyjson.h"
#include <stdio.h>
#include <time.h>

const char* get_lib_name()
{
    return "yyjson";
}

void* parse_json(const char* json, size_t length)
{
    yyjson_doc* doc = yyjson_read(json, length, 0);
    return doc;
}

const char* stringify_json(void* _doc, size_t* string_length)
{
    const yyjson_doc* doc = (const yyjson_doc*)_doc;
    yyjson_write_err err;
    return yyjson_write_opts(doc, 0, 0, string_length, &err);
}

void free_json(void* doc)
{
    yyjson_doc_free((yyjson_doc*)doc);
}

static void traverse_node(yyjson_val* node, double* ctx)
{
    yyjson_type type = yyjson_get_type(node);
    if (type == YYJSON_TYPE_ARR)
    {
        yyjson_arr_iter it;
        yyjson_arr_iter_init(node, &it);
        yyjson_val* next;
        while ((next = yyjson_arr_iter_next(&it)))
        {
            traverse_node(next, ctx);
        }
    }
    else if (type == YYJSON_TYPE_OBJ)
    {
        yyjson_obj_iter it;
        yyjson_obj_iter_init(node, &it);
        yyjson_val* key;
        while ((key = yyjson_obj_iter_next(&it))) {
            yyjson_val* val = yyjson_obj_iter_get_val(key);
            traverse_node(val, ctx);
        }
    }
    else if (type == YYJSON_TYPE_NUM)
    {
        yyjson_subtype subtype = yyjson_get_subtype(node);
        if (subtype == YYJSON_SUBTYPE_UINT)
            *ctx += yyjson_get_uint(node);
        else if (subtype == YYJSON_SUBTYPE_SINT)
            *ctx += yyjson_get_sint(node);
        else
            *ctx += yyjson_get_real(node);
    }
    else if (type == YYJSON_TYPE_BOOL)
    {
        *ctx += yyjson_get_bool(node) ? 1 : 0;
    }
}

void traverse_json(void* _doc, double* sum)
{
    yyjson_doc* doc = (yyjson_doc*)_doc;
    yyjson_val* root = yyjson_doc_get_root(doc);
    traverse_node(root, sum);
}