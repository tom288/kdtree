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

/// @brief Check whether a string slice and char* are identical
/// @param str1 The string slice
/// @param str2 The char*
/// @return true if the strings are identical, false otherwise
bool string_identical_str(string_slice str1, char* str2);

string_slice string_wordAfter(string_slice str);

/// @brief returns a shorter slice, the start of the slice moves forward to the first character after the expected text
/// text before the expected text is also not included
/// @param str the slice to be shortened
/// @return the shorter slice, the start of the slice moves forward to the first character after the expected text
string_slice string_after_expected(string_slice str, char* expected);

bool string_empty(string_slice str);
void string_print(string_slice str);

/// @brief Read the file contents at the given path and return it as a slice
/// @param path Path to the file
/// @return String slice of contents, ending in \0, which the caller must free
string_slice read_file_into_buffer(char* path);

/// @brief Use a variable number of file paths to return their merged contents
/// @param paths Variable array of char* file paths
/// @return A string slice of merged file contents, which the caller must free
string_slice merge_files(char** paths);
