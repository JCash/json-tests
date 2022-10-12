//
// clang -o test2 -DYYJSON_DISABLE_WRITER -DYYJSON_DISABLE_FAST_FP_CONV=1 example.c src/yyjson.c -Os -g
// https://github.com/ibireme/yyjson/blob/master/doc/API.md

#include "decoder.h"
#include <stdio.h>
#include <stdlib.h>

const char* get_lib_name()
{
    return "jaxon";
}

static void process_callback(json_event_t* event, void* user_data)
{
    double* ctx = (double*)user_data;
    switch(event->type)
    {
    case NIL:           break;
    case BOOLEAN:       *ctx += event->value.boolean; break;
    case INTEGER:       *ctx += event->value.integer; break;
    case DECIMAL:       *ctx += event->value.decimal; break;
    case STRING:        break;
    case START_ARRAY:   break;
    case END_ARRAY:     break;
    case KEY:           break;     /* String in the specific role of an object key. */
    case START_OBJECT:  break;
    case END_OBJECT:    break;
    default:            break;
    }
}

void* parse_json(const char* json, size_t length, double* sum)
{
    decoder_t d;
    make_decoder(&d);

    update_decoder_buffer(&d, (unsigned char*)json, length);

    json_event_t e;
    e.type = UNDEFINED;
    while(e.type < SYNTAX_ERROR) {
        decode(&d, &e);

        process_callback(&e, (void*)sum);
    }

    if (e.type == END)
        return malloc(1); // to make it think we have a document

    printf("jaxon: error code %d\n", e.type);
    return 0;
}

const char* stringify_json(void* _doc, size_t* string_length)
{
    return 0;
}

void free_json(void* doc)
{
    free(doc);
}

// static void traverse_node(cJSON* node, double* ctx)
// {
//     int type = node->type;
//     if (type == cJSON_Array || type == cJSON_Object)
//     {
//         cJSON* item = node->child;
//         while(item != NULL)
//         {
//             traverse_node(item, ctx);
//             item = item->next;
//         }
//     }
//     else if (type == cJSON_Number)
//     {
//         *ctx += node->valuedouble;
//     }
//     else if (type == cJSON_True)
//     {
//         *ctx += 1;
//     }
// }

void traverse_json(void* _doc, double* sum)
{
    // cJSON* doc = (cJSON*)_doc;
    // traverse_node(doc, sum);
    *sum = 0;
}