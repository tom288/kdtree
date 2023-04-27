#include "str_util.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // malloc, free

uint32_t string_len(string_slice str) {
    return str.firstAfter - str.first;
}

bool string_identical(string_slice str1, string_slice str2) {
    char prev[2] = { *str1.firstAfter, *str2.firstAfter };
    *str1.firstAfter = '\0';
    *str2.firstAfter = '\0';
    bool result = strcmp(str1.first, str2.first);
    *str1.firstAfter = prev[0];
    *str2.firstAfter = prev[1];
    return result;
}

string_slice string_wordAfter(string_slice str) {
    char whiteSpace[8] = " \n\t\v\b\r\f";
    char nonWhiteSpace[126 - 33];
    for(uint8_t n = 'A'; n <= '~'; n++) nonWhiteSpace[n] = n;
    nonWhiteSpace[92] = '\0';
    string_slice result;
    result.first = strpbrk(str.firstAfter, whiteSpace) + 1;
    result.firstAfter = strpbrk(str.first, nonWhiteSpace) + 1;
    return result;
}

string_slice string_after_expected(string_slice str, char* expected) {
    string_slice result;
    result.first = strstr(str.first, expected) + strlen(expected);
    result.firstAfter = str.firstAfter;
    return result;
}
