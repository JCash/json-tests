#include "zzzjson.h"
#include <stdio.h>
#include <time.h>

const char* get_lib_name()
{
  return "zzzjson";
}

void* parse_json(const char* json, size_t length, double* sum)
{
    zj_Allocator* A = zj_NewAllocator();
    zj_Value* root = zj_NewValue(A);

    bool ret = zj_ParseLen(root, json, length);
    return ret ? root : 0;
}

const char* stringify_json(void* _doc, size_t* string_length)
{
  printf("Writing not yet implemented\n");
  return 0;
}

void free_json(void* doc)
{
  zj_Value* root = (zj_Value*)doc;
  zj_Allocator* A = root->A;
  zj_ReleaseAllocator(A);
}

void traverse_json(void* _doc, double* sum)
{
}