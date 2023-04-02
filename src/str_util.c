#include "str_util.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // malloc, free

uint32_t string_len(char* str) {
    return strlen(str);
}

char* string_substr(char* str, uint32_t startIndex, uint32_t len) {
    size_t length = len;
    if (length <= startIndex) length = strlen(str) - startIndex;
    char* result = malloc(length + 1);
    memcpy(result, str[startIndex], length);
    result[length] = '\0';
    return result;
}

// ---

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

uint32_t string_skipLineBreak(char* str, uint32_t startIndex) {
    while (
        str[startIndex] == '\n' || 
        str[startIndex] == '\r'
    ) startIndex++;
    return startIndex;
}

// ---

uint32_t string_skipChars(char* str, char* skipChars, uint32_t startIndex) {
    uint32_t i = startIndex;
    for (; str[i] != '\0'; ++i)
    {
        bool skip = false;
        for (uint8_t m = 0; m < strlen(skipChars); m++) {
            if (str[i] == skipChars[m]) 
            {
                skip = true;
                break;
            }
        }
        if (!skip) break;
    }
    return i;
}

uint32_t string_skipWhitespace(char* str, uint32_t startIndex) {
    return string_skipChars(str, " \n\t\v\b\r\f", startIndex);
}
