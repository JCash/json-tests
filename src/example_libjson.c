//
// clang -o test2 -DYYJSON_DISABLE_WRITER -DYYJSON_DISABLE_FAST_FP_CONV=1 example.c src/yyjson.c -Os -g
// https://github.com/ibireme/yyjson/blob/master/doc/API.md

#include "json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* get_lib_name()
{
    return "libjson";
}

static int parser_callback(void *userdata, int type, const char *data, uint32_t length)
{
    double* sum = (double*)userdata;
	switch (type) {
	case JSON_OBJECT_BEGIN:
	case JSON_ARRAY_BEGIN:
		//fprintf(output, "entering %s\n", (type == JSON_ARRAY_BEGIN) ? "array" : "object");
		break;
	case JSON_OBJECT_END:
	case JSON_ARRAY_END:
		//fprintf(output, "leaving %s\n", (type == JSON_ARRAY_END) ? "array" : "object");
		break;
	case JSON_KEY:      break;
	case JSON_STRING:   break;
	case JSON_INT:      *sum += atoi(data); break;
	case JSON_FLOAT:    *sum += atof(data); break;
	case JSON_NULL:
		//fprintf(output, "constant null\n");
        break;
	case JSON_TRUE:     *sum += 1; break;
	case JSON_FALSE:
		//fprintf(output, "constant false\n");
        break;
	}
    return 0;
}

void* parse_json(const char* json, size_t length, double* sum)
{
    json_config config;
	memset(&config, 0, sizeof(json_config));

	json_parser parser;

    int ret = json_parser_init(&parser, &config, parser_callback, (void*)sum);
	if (ret) {
		printf("libjson: error: initializing parser failed (code=%d)\n", ret);
		return 0;
	}

	ret = json_parser_string(&parser, json, length, 0);
	if (ret) {
        printf("libjson: failed to parse string: %d\n", ret);
		goto fail;
    }

    ret = json_parser_is_done(&parser);
	if (!ret) {
        printf("libjson: error code %d\n", ret);
		goto fail;
    }

    json_parser_free(&parser);
    return malloc(1); // to make it think we have a document

fail:
    json_parser_free(&parser);
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