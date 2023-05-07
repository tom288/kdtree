#pragma once
#include <stdint.h>
#include <stdbool.h>

/// @brief pointer to start and end of substring
/// last = (firstAfter - 1)
/// (firstAfter - first) = length
/// HelloWorld
/// sub: elloWo
/// first: 1
/// last: 6
/// firstAfter: 7
/// If the slice ends on the last character of the string,
/// then firstAfter will be the strings ending null character (/0).
/// Needs to be passed as pointer if it will be modified.
typedef struct {
    char* first; // pointer to character
    char* firstAfter; // pointer to character
} string_slice;

/// @brief yields the length of a string slice, use strlen for char*
/// @param str string to be measured
/// @return length of the string
uint32_t string_len(string_slice str);

/// @brief check whether two string slices are identical, use strcmp for char*
/// @param str1 the first string slice to be compared
/// @param str2 the second string slice to be compared
/// @return true if the string slices are identical, false otherwise
bool string_identical(string_slice str1, string_slice str2);

string_slice string_wordAfter(string_slice str);

/// @brief returns a shorter slice, the start of the slice moves forward to the first character after the expected text
/// text before the expected text is also not included
/// @param str the slice to be shortened
/// @return the shorter slice, the start of the slice moves forward to the first character after the expected text
string_slice string_after_expected(string_slice str, char* expected);

bool string_empty(string_slice str);
void string_print(string_slice str);
