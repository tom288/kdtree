#include "str_util.h"
#include "ctype.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // malloc, free

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

string_slice string_after_expected(string_slice str, char* expected) {
    string_slice result;
    result.first = strstr(str.first, expected) + strlen(expected);
    result.firstAfter = str.firstAfter;
    return result;
}

bool string_empty(string_slice str) {
    return str.first == str.firstAfter;
}

void string_print(string_slice str) {
    for (uint32_t n = 0; n < str.firstAfter - str.first; n++)
        printf("%c", str.first[n]);
}
