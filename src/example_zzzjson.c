
#include "zzzjson.h"
#include <stdio.h>
#include <time.h>

// static void PrintIndent(int indent)
// {
//   for (int i = 0; i < indent; ++i)
//     printf("  ");
// }

// void Print(zj_Value *value, int indent)
// {
//     PrintIndent(indent);

//     zj_Size size = 0;
//     const zj_JSONType *t = zj_Type(value);
//     switch (*t)
//     {
//     case zj_JSONTypeArray:
//     {
//         printf("[");
//         zj_Value *next = zj_Begin(value);
//         while (next != 0)
//         {
//             Print(next, indent+1);
//             printf(",");
//             next = zj_Next(next);
//         }
//         printf("]");
//     }
//     break;
//     case zj_JSONTypeObject:
//     {
//         printf("{\n");
//         zj_Value *next = zj_Begin(value);
//         while (next != 0)
//         {
//             const char* key = zj_GetKeyFast(next, &size);
//             PrintIndent(indent+1);
//             printf("\"%.*s\": ", size, key);
//             Print(next, indent+1);
//             printf("\n");
//             next = zj_Next(next);
//         }
//         PrintIndent(indent); printf("}");
//     }
//     break;
//     case zj_JSONTypeString:
//     {
//       const char* s = zj_GetStrFast(value, &size);
//       printf("\"%.*s\"", size, s);
//     }
//     break;
//     case zj_JSONTypeNumber: printf("%g", *zj_GetDouble(value)); break;
//     case zj_JSONTypeBool:   printf("%s", *(zj_GetBool(value))?"true":"false"); break;
//     case zj_JSONTypeNull:   printf("null"); break;
//     }
// }


const char* get_lib_name()
{
  return "zzzjson";
}

void* parse_json(const char* json, size_t length)
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