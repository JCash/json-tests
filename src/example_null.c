#include <stdio.h>
#include <time.h>

const char* get_lib_name()
{
    return "null";
}

void* parse_json(const char* json, size_t length)
{
    return (void*)(uintptr_t)1;
}

const char* stringify_json(void* _doc, size_t* string_length)
{
    printf("Writing not yet implemented\n");
    return 0;
}

void free_json(void* doc)
{
    (void)doc;
}

void traverse_json(void* _doc, double* sum)
{
}