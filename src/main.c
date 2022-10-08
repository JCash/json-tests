
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

const char* get_lib_name();
extern void* parse_json(const char* json, size_t length);
extern void traverse_json(void* _doc, double* sum);
extern const char* stringify_json(void* _doc, size_t* string_length);
extern void free_json(void* doc);

static const char* read_file(const char* path, size_t* size)
{
    FILE* file = fopen(path, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Expected file \"%s\" not found", path);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* buffer = (char*)malloc(len + 1);

    if (buffer == NULL)
    {
        fprintf(stderr, "Unable to allocate memory for file");
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, len, file);
    fclose(file);
    buffer[len] = '\0';

    *size = (size_t)len;
    return (const char*)buffer;
}

static double calc_time(clock_t t[2])
{
    return (double)(t[1] - t[0]) / (double)CLOCKS_PER_SEC;
}

static void output(const char* format, ...)
{
    char buffer[1024];
    va_list argptr;
    va_start(argptr, format);
    vsnprintf(buffer, sizeof(buffer), format, argptr);
    va_end(argptr);
    write(STDERR_FILENO, buffer, strlen(buffer));
    fflush(stderr);
}

int main(int argc, char** argv)
{
    const char* json = 0;
    size_t size = 0;
    if (argc >= 2)
        json = read_file(argv[1], &size);

    if (json == 0)
    {
        printf("No json file specified\n");
        return -1;
    }

    clock_t t_parse[2];
    clock_t t_stringify[2];
    clock_t t_traverse[2];

    t_parse[0] = clock();
    void* doc = parse_json(json, size);
    t_parse[1] = clock();

    if (!doc)
    {
        free((void*)json);
        output("%s: Failed reading json %s\n", get_lib_name(), argv[1]);
        return 1;
    }

    t_traverse[0] = clock();
    double sum = 0;
    traverse_json(doc, &sum);
    t_traverse[1] = clock();
    printf("traverse: sum: %g\n", sum);

    // t_stringify[0] = clock();
    // size_t string_length;
    // const char* s = stringify_json(doc, &string_length);
    // t_stringify[1] = clock();

    // if (s) {
    //     size_t numchars = 32;
    //     if (string_length < numchars)
    //         numchars = string_length;
    //     printf("string[0:%zu]: '%.*s\n", numchars-1, (int)numchars, s);
    // }
    // free((void*)s);
    t_stringify[0] = clock();
    t_stringify[1] = t_stringify[1];

    free_json(doc);
    printf("%s:\tSize: %u  Parse: %fs Stringify: %fs Traverse: %fs\n", get_lib_name(), (uint32_t)size,
        calc_time(t_parse), calc_time(t_stringify), calc_time(t_traverse));

    free((void*)json);
    fflush(stdout);
    fflush(stderr);
    return 0;
}
