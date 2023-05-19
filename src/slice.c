#include "slice.h"
#include "ctype.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // malloc, free
#include <stb_ds.h>

uint32_t slice_len(Slice str) {
    return str.firstAfter - str.first;
}

bool slice_eq(Slice str1, Slice str2) {
    char prev[2] = {*str1.firstAfter, *str2.firstAfter};
    *str1.firstAfter = '\0';
    if (*str2.firstAfter) *str2.firstAfter = '\0';
    bool result = !strcmp(str1.first, str2.first);
    *str1.firstAfter = prev[0];
    if (prev[1]) *str2.firstAfter = prev[1];
    return result;
}

bool slice_eq_str(Slice str1, char* str2) {
    char prev = *str1.firstAfter;
    *str1.firstAfter = '\0';
    bool result = !strcmp(str1.first, str2);
    *str1.firstAfter = prev;
    return result;
}

Slice slice_word_after(Slice str) {
    str.first = str.firstAfter;
    while (!isspace(*str.first)) {
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

void slice_print(Slice str) {
    for (uint32_t n = 0; n < str.firstAfter - str.first; n++) {
        printf("%c", str.first[n]);
    }
}

Slice slice_from_path(char* path)
{
    FILE* const file = fopen(path, "rb");
    Slice null_slice = { NULL };

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
        if (*paths[i].firstAfter) *paths[i].firstAfter = '\0';
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
