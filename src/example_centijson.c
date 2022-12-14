//
// clang -o test2 -DYYJSON_DISABLE_WRITER -DYYJSON_DISABLE_FAST_FP_CONV=1 example.c src/yyjson.c -Os -g
// https://github.com/ibireme/yyjson/blob/master/doc/API.md

#include "json.h"
#include <stdio.h>
#include <stdlib.h>

const char* get_lib_name()
{
    return "centijson";
}

static int process_callback(JSON_TYPE type, const char* data, size_t data_size, void* user_data)
{
    double* ctx = (double*)user_data;
    switch(type)
    {
    case JSON_NULL: break;
    case JSON_FALSE: break;
    case JSON_TRUE:
        *ctx += 1;
        break;
    case JSON_NUMBER:
        {
            int i32, u32, i64, u64;
            json_analyze_number(data, data_size, &i32, &u32, &i64, &u64);
            if (i64)
                *ctx += json_number_to_int64(data, data_size);
            else if (u64)
                *ctx += json_number_to_uint64(data, data_size);
            else if (i32)
                *ctx += json_number_to_int32(data, data_size);
            else if (u32)
                *ctx += json_number_to_uint32(data, data_size);
            else {
                double d;
                int ret = json_number_to_double(data, data_size, &d);
                if (ret) {
                    printf("centijson: %s\n", json_error_str(ret));
                    return ret;
                } else {
                    *ctx += d;
                }
            }
        }
        break;
    case JSON_STRING: break;
    case JSON_ARRAY_BEG: break;
    case JSON_ARRAY_END: break;
    case JSON_KEY: break;     /* String in the specific role of an object key. */
    case JSON_OBJECT_BEG: break;
    case JSON_OBJECT_END: break;
    }
    return 0;
}


static void printIndent(int indent)
{
    for (int i = 0; i < indent; ++i)
    {
        printf("  ");
    }
}

static int print_callback(JSON_TYPE type, const char* data, size_t data_size, void* user_data)
{
    int* indent = (int*)user_data;

    printIndent(*indent);

    switch(type)
    {
    case JSON_NULL:     printf("nil"); break;
    case JSON_TRUE:     printf("true (bool)"); break;
    case JSON_FALSE:    printf("false (bool)"); break;
    case JSON_NUMBER:
        {
            int i32, u32, i64, u64;
            json_analyze_number(data, data_size, &i32, &u32, &i64, &u64);
            if (i64)
                printf("%lld (int64)", json_number_to_int64(data, data_size));
            else if (u64)
                printf("%llu (uint64)", json_number_to_uint64(data, data_size));
            else if (i32)
                printf("%d (int32)", json_number_to_int32(data, data_size));
            else if (u32)
                printf("%u (uint32)", json_number_to_uint32(data, data_size));
            else {
                double d;
                int ret = json_number_to_double(data, data_size, &d);
                if (ret) {
                    printf("centijson: %s\n", json_error_str(ret));
                    return ret;
                } else {
                    printf("%g (number)", d);
                }
            }
        }
        break;
    case JSON_STRING:   printf("%.*s (str)", (uint32_t)data_size, data); break;
    case JSON_KEY:      printf("%.*s (key)", (uint32_t)data_size, data); break;
    case JSON_ARRAY_BEG:
        (*indent)++;
        printf("[");
        break;
    case JSON_ARRAY_END:
        (*indent)--;
        printf("]");
        break;
    case JSON_OBJECT_BEG:
        (*indent)++;
        printf("{");
        break;
    case JSON_OBJECT_END:
        (*indent)--;
        printf("}");
        break;
    default:            break;
    }

    printf("  t: %d\n", type);
    return 0;
}


void* parse_json(const char* json, size_t length, double* sum)
{
    JSON_CONFIG config;
    json_default_config(&config);

    JSON_CALLBACKS callbacks;
    callbacks.process = process_callback;
    int ret = json_parse(json, length, &callbacks, &config, sum, 0);

    // int indent = 0;
    // callbacks.process = print_callback;
    // int ret = json_parse(json, length, &callbacks, &config, &indent, 0);
    if (ret) {
        printf("centijson: %s\n", json_error_str(ret));
        return 0;
    }

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