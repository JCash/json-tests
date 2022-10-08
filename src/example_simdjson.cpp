#include "simdjson.h"
#include <stdio.h>
#include <time.h>

extern "C"
const char* get_lib_name()
{
    return "simdjson";
}

struct Document
{
    simdjson::dom::parser parser;
    simdjson::dom::element doc;
};

extern "C"
void* parse_json(const char* json, size_t length)
{
    Document* doc = new Document();

    auto error = doc->parser.parse(json,length).get(doc->doc);
    if (error) { printf("Error: %s\n", error_message(error)); }
    return (void*)doc;
}

extern "C"
const char* stringify_json(void* _doc, size_t* string_length)
{
    printf("Writing not yet implemented\n");
    return 0;
}

extern "C"
void free_json(void* doc)
{
    delete (Document*)doc;
    (void)doc;
}

//   ARRAY = '[',     ///< dom::array
//   OBJECT = '{',    ///< dom::object
//   INT64 = 'l',     ///< int64_t
//   UINT64 = 'u',    ///< uint64_t: any integer that fits in uint64_t but *not* int64_t
//   DOUBLE = 'd',    ///< double: Any number with a "." or "e" that fits in double.
//   STRING = '"',    ///< std::string_view
//   BOOL = 't',      ///< bool
//   NULL_VALUE = 'n' ///< null


static void traverse_node(simdjson::dom::element* node, double* ctx)
{
    simdjson::dom::element_type type = node->type();
    if (type == simdjson::dom::element_type::ARRAY)
    {
        simdjson::dom::array array = node->get_array();
        simdjson::dom::array::iterator it = array.begin();
        for (;it != array.end(); ++it)
        {
            simdjson::dom::element item = *it;
            traverse_node(&item, ctx);
        }
    }
    else if (type == simdjson::dom::element_type::OBJECT)
    {
        simdjson::dom::object object = node->get_object();
        simdjson::dom::object::iterator it = object.begin();
        for (;it != object.end(); ++it)
        {
            simdjson::dom::key_value_pair kv_pair = *it;
            traverse_node(&kv_pair.value, ctx);
        }
    }
    else if (type == simdjson::dom::element_type::INT64)
    {
        *ctx += node->get_int64();
    }
    else if (type == simdjson::dom::element_type::UINT64)
    {
        *ctx += node->get_uint64();
    }
    else if (type == simdjson::dom::element_type::DOUBLE)
    {
        *ctx += node->get_double();
    }
    else if (type == simdjson::dom::element_type::BOOL)
    {
        *ctx += node->get_bool() ? 1 : 0;
    }
}

extern "C"
void traverse_json(void* _doc, double* sum)
{
    Document* doc = (Document*)_doc;
    traverse_node(&doc->doc, sum);
}