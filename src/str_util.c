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

uint32_t string_skipWhitespace(char* str, uint32_t startIndex) {
    return string_skipChars(str, " \n\t\v\b\r\f", startIndex);
}

uint32_t string_skipUntilWhitespace(char* str, uint32_t startIndex) {
    char whiteSpaceEOS[8] = " \n\t\v\b\r\f\0"; // whitespace and end of string, cannot be returned
    uint8_t whiteSpaceEOSLen = 8;
    uint32_t n = startIndex;
    bool ok = true;
    while (ok) {
        for (uint8_t m = 0; m < whiteSpaceEOSLen; m++) {
            if (str[n] == whiteSpaceEOS[m]) ok = false;
        }
        n++;
    }
    return n;
}

uint32_t string_after_expected(char* str, char* expected, uint32_t startIndex) {
    uint32_t expected_len = strlen(expected);
    uint32_t n = startIndex;
    bool match;
    do {
        char* substr = string_substr(expected, n, expected_len);
        match = strcmp(substr, expected);
        free(substr);
        n++;
    } while (!match);
    return string_substr(str, n + strlen(expected), 0);
}

// ---

uint32_t string_skipLineBreak(char* str, uint32_t startIndex) {
    while (
        str[startIndex] == '\n' ||
        str[startIndex] == '\r'
    ) startIndex++;
    return startIndex;
}
