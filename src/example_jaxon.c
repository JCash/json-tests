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

// static void printIndent(int indent)
// {
//     for (int i = 0; i < indent; ++i)
//     {
//         printf("  ");
//     }
// }

// static void print_callback(json_event_t* event, void* user_data)
// {
//     int* indent = (int*)user_data;

//     printIndent(*indent);

//     switch(event->type)
//     {
//     case NIL:           printf("nil"); break;
//     case BOOLEAN:       printf("%d (bool)", event->value.boolean?1:0); break;
//     case INTEGER:       printf("%ld (integer)", event->value.integer); break;
//     case DECIMAL:       printf("%g (number)", event->value.decimal); break;
//     case STRING:        printf("%.*s (str)", (uint32_t)event->value.string.size, event->value.string.buffer); break;
//     case START_ARRAY:
//         (*indent)++;
//         printf("[");
//         break;
//     case END_ARRAY:
//         (*indent)--;
//         printf("]");
//         break;
//     case KEY:           printf("%.*s (key)", (uint32_t)event->value.string.size, event->value.string.buffer); break;
//     case START_OBJECT:
//         (*indent)++;
//         printf("{");
//         break;
//     case END_OBJECT:
//         (*indent)--;
//         printf("}");
//         break;
//     default:            break;
//     }

//     printf("  t: %d\n", event->type);
// }

void* parse_json(const char* json, size_t length, double* sum)
{
    decoder_t d;
    make_decoder(&d);

    update_decoder_buffer(&d, (unsigned char*)json, length);

    //int indent = 0;
    json_event_t e;
    e.type = UNDEFINED;
    while(e.type < SYNTAX_ERROR) {
        decode(&d, &e);

        process_callback(&e, (void*)sum);
        //print_callback(&e, (void*)&indent);
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

void traverse_json(void* _doc, double* sum)
{
    *sum = 0;
}