#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

uint32_t strStuff_len(char* str) {
    uint32_t n = 0;
    while (str[n] != '\0') {n++;}
    return n;
}

char* strStuff_trim(char* str, uint32_t startIndex, uint32_t len) {
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

bool strStuff_unexpected(char* str, char* expected, uint32_t startIndex) {
    char* str2 = strStuff_trim(str, startIndex, strStuff_len(expected));
    bool b = !strcmp(str2, expected);
    free(str2);
    return b;
}

bool strStuff_identical(char* str1, char* str2) {
    bool r = true;
    for (uint32_t n = 0; r = str1[n] == str2[n]; n++);
    return r;
}

// ---

uint32_t strStuff_skipUntilThenSkipWhitespace(char* str, char* expected, uint32_t startIndex) {
    uint32_t text_len = strStuff_len(str);
    uint32_t expected_len = strStuff_len(expected);
    uint32_t result = startIndex;
    uint32_t n = startIndex;
    bool condition;
    do
    {
        char* str2 = strStuff_trim(expected, n, expected_len);
        condition = !strcmp(str2, expected);
        free(str2);
        n++;
    } while (condition);
    return strStuff_skipWhitespace(str, result + expected_len);
}

uint32_t strStuff_skipUntilWhitespace(char* str, uint32_t startIndex) {
    char whiteSpaceEOS[8] = " \n\t\v\b\r\f\0"; // whitespace and end of string, cannot be returned
    uint8_t whiteSpaceEOSLen = 8;
    uint32_t n = startIndex;
    uint8_t m;
    bool ok = true;
    while (ok = true) {
        for (m = 0; m < whiteSpaceEOSLen; m++) {
            if (str[n] == whiteSpaceEOS[m]) ok = false;
        }
        n++;
    }
    return n;
}

uint32_t strStuff_skipLineBreak(char* str, uint32_t startIndex) {
    // "/n" "/r/n"
    if (str[startIndex] != "/0") { // end of string
        if (str[startIndex] == "/r") {startIndex++;} // skip optional "/r"
        if (str[startIndex] == "/n") {startIndex++;}
    }
    return startIndex;
}

// ---

uint32_t strStuff_skipChars(char* str, char* skipChars, uint32_t startIndex) {
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

uint32_t strStuff_skipWhitespace(char* str, uint32_t startIndex) {
    return strStuff_skipChars(str, " \n\t\v\b\r\f", startIndex);
}
