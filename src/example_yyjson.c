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


// Type GetType(Node* node)
// {
//     yyjson_type type = yyjson_get_type((yyjson_val*)node);
//     switch (type)
//     {
//         case YYJSON_TYPE_BOOL:  return TYPE_BOOL;
//         case YYJSON_TYPE_NUM:   return TYPE_NUMBER;
//         case YYJSON_TYPE_STR:   return TYPE_STRING;
//         case YYJSON_TYPE_ARR:   return TYPE_ARRAY;
//         case YYJSON_TYPE_OBJ:   return TYPE_OBJECT;
//         case YYJSON_TYPE_NONE:
//         case YYJSON_TYPE_RAW:
//         case YYJSON_TYPE_NULL:
//         default:
//             return TYPE_NULL;
//     }
// }

// NumberType GetNumberType(Node* node)
// {
//     yyjson_subtype subtype = yyjson_get_subtype(node);
//     switch(subtype)
//     {
//     case YYJSON_SUBTYPE_UINT:   return NUMBERTYPE_UINT64;
//     case YYJSON_SUBTYPE_SINT:   return NUMBERTYPE_INT64;
//     case YYJSON_SUBTYPE_REAL:
//     default:
//         return NUMBERTYPE_FLOAT64;
//     }
// }
// uint32_t    GetObjectCount(Node* node)                  { return yyjson_obj_size(node); }
// uint32_t    GetArraySize(Node* node)                    { return yyjson_arr_size(node); }
// Node*       GetArrayElement(Node* node, uint32_t index) { return yyjson_arr_get(node, index); }

// const char* GetString(Node* node)   { return yyjson_get_str(node); }
// bool        GetBool(Node* node)     { return yyjson_get_bool(node); }
// int         GetInt32(Node* node)    { return yyjson_get_int(node); }
// int64_t     GetInt64(Node* node)    { return yyjson_get_sint(node); }
// uint64_t    GetUInt64(Node* node)   { return yyjson_get_uint(node); }
// double      GetFloat64(Node* node)  { return yyjson_get_real(node); }

// Iterator Iterate(Node* node)
// {
//     assert(node);
//     Iterator it;
//     memset(&it, 0, sizeof(Iterator));
//     it.m_Initial = node;
//     it.m_Type = GetType(node);
//     return it;
// }

// bool IterateNext(Iterator* it)
// {
//     if (it->m_Type == TYPE_ARRAY)
//     {
//         if (it->m_Initial) {
//             it->m_Key = it->m_Initial;
//             yyjson_arr_iter_init(it->m_Initial, &it->m_ArrayIter);
//             it->m_Initial = 0;
//         }
//         it->m_Value = yyjson_arr_iter_next(&it->m_ArrayIter);
//     }
//     else
//     {
//         if (it->m_Initial)
//         {
//             yyjson_obj_iter_init(it->m_Initial, &it->m_ObjectIter);
//             it->m_Initial = 0;
//         }
//         it->m_Key = yyjson_obj_iter_next(&it->m_ObjectIter);
//         it->m_Value = yyjson_obj_iter_get_val(it->m_Key); // handles null
//     }

//     return it->m_Value != 0;
// }

    // typedef struct yyjson_obj_iter InternalObjectIter;
    // typedef struct yyjson_arr_iter InternalArrayIter;

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