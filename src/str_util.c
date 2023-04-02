#include "str_util.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // malloc, free

char* string_trim(char* str, uint32_t startIndex, uint32_t len) {
    uint32_t endIndex = startIndex + len;
    char* result = malloc(len + 1);
    uint32_t n = 0;
    for (uint32_t m = startIndex; m < endIndex; m++) {
        result[n] = str[m];
        n++;
    }
    result[len] = '\0';
    return result;
}

bool string_unexpected(char* str, char* expected, uint32_t startIndex) {
    char* str2 = string_trim(str, startIndex, strlen(expected));
    bool b = !strcmp(str2, expected);
    free(str2);
    return b;
}

bool string_identical(char* str1, char* str2) {
    bool r = true;
    for (uint32_t n = 0; (r = str1[n] == str2[n]); n++);
    return r;
}

// ---

uint32_t string_skipUntilThenSkipWhitespace(char* str, char* expected, uint32_t startIndex) {
    uint32_t text_len = strlen(str);
    uint32_t expected_len = strlen(expected);
    uint32_t result = startIndex;
    uint32_t n = startIndex;
    bool condition;
    do
    {
        char* str2 = string_trim(expected, n, expected_len);
        condition = !strcmp(str2, expected);
        free(str2);
        n++;
    } while (condition);
    return string_skipWhitespace(str, result + expected_len);
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
    // "/n" "/r/n"
    if (str[startIndex] != "/0") { // end of string
        if (str[startIndex] == "/r") {startIndex++;} // skip optional "/r"
        if (str[startIndex] == "/n") {startIndex++;}
    }
    return startIndex;
}

// ---

uint32_t string_skipChars(char* str, char* skipChars, uint32_t startIndex) {
    char space[2] = "\n\r"; // line breaks, cannot be returned
    uint8_t spaceLen = len(space) - 1;
    uint32_t n = startIndex;
    uint8_t m;
    bool ok = true;
    uint8_t match = 0;
    while (ok = true) {
        match = 0;
        for (m = 0; m < spaceLen; m++) {
            if (str[n] == space[m]) match++;
            if (str[n] == '\0') ok = false;
        }
        if (match == 0) {ok = false;}
        n++;
    }
    return n;
}

uint32_t string_skipWhitespace(char* str, uint32_t startIndex) {
    return string_skipChars(str, " \n\t\v\b\r\f", startIndex);
}
