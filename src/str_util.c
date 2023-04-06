#include "str_util.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // malloc, free

uint32_t string_len(string_slice str) {
    return str.firstAfter - str.first;
}

// ---

string_slice string_skipChars(string_slice str, char* skipChars) {
    char* c = str.first;
    for (; *c != '\0'; ++c) {
        bool skip = false;
        for (uint8_t m = 0; m < strlen(skipChars); m++) {
            if (*c == skipChars[m]) {
                skip = true;
                break;
            }
        }
        if (!skip) break;
    }
    string_slice result;
    result.first = c;
    result.firstAfter = str.firstAfter;
    return result;
}

string_slice string_skipWhitespace(string_slice str) {
    return string_skipChars(str, " \n\t\v\b\r\f");
}

string_slice string_skipNonWhitespace(string_slice str) {
    char whiteSpaceEOS[7] = " \n\t\v\b\r\f";
    string_slice result;
    result.first = strpbrk(str.first, whiteSpaceEOS) + 1;
    result.firstAfter = str.firstAfter;
    return result;
}

// ---

string_slice string_after_expected(string_slice str, char* expected) {
    string_slice result;
    result.first = strstr(str.first, expected) + strlen(expected);
    result.firstAfter = str.firstAfter;
    return result;
}

string_slice string_skipLineBreak(string_slice str) {
    // "/r/n" or "/n"
    string_slice result;
    result.first = str.first;
    result.firstAfter = str.firstAfter;
    if (str.first == '\r') result.first++;
    if (str.first == "\n") result.first++;
    return result;
}
