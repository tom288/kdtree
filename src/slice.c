#include "slice.h"
#include "utility.h"
#include <ctype.h> // isspace
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // malloc, free
#include <stb_ds.h>

extern char* rule_dir;

uint32_t slice_len(Slice str)
{
    return str.firstAfter - str.first;
}

bool slice_eq_str(Slice str1, char* str2)
{
    char prev = *str1.firstAfter;
    *str1.firstAfter = '\0';
    bool result = !strcmp(str1.first, str2);
    *str1.firstAfter = prev;
    return result;
}

bool slice_eq(Slice str1, Slice str2)
{
    char prev = *str2.firstAfter;
    if (prev) *str2.firstAfter = '\0';
    bool result = slice_eq_str(str1, str2.first);
    if (prev) *str2.firstAfter = prev;
    return result;
}

Slice slice_word_after(Slice str)
{
    str.first = str.firstAfter;
    while (!isspace(*str.first))
    {
        if (!*str.first)
        {
            str.firstAfter = str.first;
            return str;
        }
        str.first++;
    }

    while (isspace(*str.first)) str.first++;
    str.firstAfter = str.first;
    if (!*str.first) return str;

    while (!isspace(*str.firstAfter))
    {
        if (!*str.firstAfter)
        {
            str.first = str.firstAfter;
            return str;
        }
        str.firstAfter++;
    }
    return str;
}

void slice_print(Slice str)
{
    for (uint32_t n = 0; n < str.firstAfter - str.first; n++)
    {
        printf("%c", str.first[n]);
    }
}

Slice slice_from_path(char* path)
{
    Slice null_slice = { NULL };

    if (!rule_dir)
    {
        fprintf(stderr, "Rule directory undefined for %s\n", path);
        return null_slice;
    }

    char* file_path = malloc(1 + strlen(rule_dir) + strlen(path));

    if (!file_path)
    {
        fprintf(
            stderr,
            "Failed to malloc for rule directory during compilation of %s\n",
            path
        );
        return null_slice;
    }

    strcpy(file_path, rule_dir);
    strcpy(file_path + strlen(rule_dir), path);

    FILE* const file = fopen(file_path, "rb");

    if (!file)
    {
        fprintf(stderr, "Failed to fopen ");
        perror(file_path);
    }

    free(file_path);
    file_path = NULL;

    if (!file) return null_slice;

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
    return (Slice) {
        .first = buffer,
        .firstAfter = buffer + file_size,
    };
}

Slice slice_from_paths(Slice* paths)
{
    Slice* buffers = NULL;
    size_t total_bytes = 0;

    for (size_t i = 0; i < arrlenu(paths); ++i)
    {
        char prev = *paths[i].firstAfter;
        if (prev) *paths[i].firstAfter = '\0';
        arrput(buffers, slice_from_path(paths[i].first));
        if (prev) *paths[i].firstAfter = prev;
        total_bytes += slice_len(buffers[i]);
    }

    Slice big;
    big.first = calloc(1, total_bytes + 1),
    big.firstAfter = big.first;

    for (size_t i = 0; i < arrlenu(buffers); ++i)
    {
        // If one of the files has failed to read we must not call strcpy
        if (slice_len(buffers[i]))
        {
            // This (correctly) assumes the RHS has a null character afterwards
            strcpy(big.firstAfter, buffers[i].first);
            big.firstAfter += slice_len(buffers[i]);
            free(buffers[i].first);
        }
    }

    return big;
}

char* dir_path(char* argv0, char* sub_dir)
{
    const size_t dir_len = max(
        max(
            strrchr(argv0, '/'),
            strrchr(argv0, '\\')
        ) + 1,
        argv0
    ) - argv0;

    char* full_path = malloc(1 + dir_len + strlen(sub_dir));

    if (!full_path)
    {
        fprintf(stderr, "Failed to malloc for %s\n", sub_dir);
        return NULL;
    }

    memcpy(full_path, argv0, dir_len);
    strcpy(full_path + dir_len, sub_dir);

    return full_path;
}
