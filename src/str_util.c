#include "str_util.h"
#include "ctype.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // malloc, free
#include <stb_ds.h>

uint32_t string_len(string_slice str) {
    return str.firstAfter - str.first;
}

bool string_identical(string_slice str1, string_slice str2) {
    char prev[2] = {*str1.firstAfter, *str2.firstAfter};
    *str1.firstAfter = '\0';
    if (*str2.firstAfter) *str2.firstAfter = '\0';
    bool result = !strcmp(str1.first, str2.first);
    *str1.firstAfter = prev[0];
    if (prev[1]) *str2.firstAfter = prev[1];
    return result;
}

bool string_identical_str(string_slice str1, char* str2) {
    char prev = *str1.firstAfter;
    *str1.firstAfter = '\0';
    bool result = !strcmp(str1.first, str2);
    *str1.firstAfter = prev;
    return result;
}

string_slice string_wordAfter(string_slice str) {
    str.first = str.firstAfter;
    while (!isspace(*str.first)){
        if (*str.first == '\0') {
            str.firstAfter = str.first;
            return str;
        }
        str.first++;
    }
    while (isspace(*str.first)) str.first++;
    str.firstAfter = str.first;
    if (*str.first == '\0') return str;
    while (!isspace(*str.firstAfter)) {
        if (*str.firstAfter == '\0') {
            str.first = str.firstAfter;
            return str;
        }
        str.firstAfter++;
    }
    return str;
}

bool string_empty(string_slice str) {
    return str.first == str.firstAfter;
}

void string_print(string_slice str) {
    for (uint32_t n = 0; n < str.firstAfter - str.first; n++)
        printf("%c", str.first[n]);
}

string_slice read_file_into_buffer(char* path)
{
    FILE* const file = fopen(path, "rb");
    string_slice null_slice = { NULL };

    if (!file) {
        fprintf(stderr, "Failed to fopen ");
        perror(path);
        return null_slice;
    }

    fseek(file, 0L, SEEK_END);
    const long file_size = ftell(file);
    rewind(file);

    char* const buffer = calloc(1, file_size + 1);
    if (fread(buffer, file_size, 1, file) != 1)
    {
        fclose(file);
        free(buffer);
        fprintf(stderr, "Failed to fread for %s\n", path);
        return null_slice;
    }

    fclose(file);
    return (string_slice) {
        .first = buffer,
        .firstAfter = buffer + file_size,
    };
}

string_slice merge_files(string_slice* paths)
{
    string_slice* buffers = NULL;
    size_t total_bytes = 0;

    for (size_t i = 0; i < arrlenu(paths); ++i)
    {
        arrput(buffers, read_file_into_buffer(paths[i].first));
        total_bytes += string_len(buffers[i]);
    }

    string_slice big;
    big.first = calloc(1, total_bytes + 1),
    big.firstAfter = big.first;

    for (size_t i = 0; i < arrlenu(buffers); ++i)
    {
        // If one of the files has failed to read we must not call strcpy
        if (string_len(buffers[i]))
        {
            // This (correctly) assumes the RHS has a null character afterwards
            strcpy(big.firstAfter, buffers[i].first);
            big.firstAfter += string_len(buffers[i]);
            free(buffers[i].first);
        }
    }

    return big;
}
