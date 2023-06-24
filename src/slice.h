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
} Slice;

typedef struct {
    bool lineBreak;
    uint8_t tabsAfterLineBreak;
    Slice wordAfter;
} SliceWordAfterInfo;

/// @brief Yields slice length, use strlen for char*
/// @param str String slice to be measured
/// @return length of the string slice
uint32_t slice_len(Slice str);

/// @brief Construct a slice from a string
/// @param str String input
/// @return Slice output
Slice slice_from_str(char* str);

/// @brief Check whether a string slice and char* are identical
/// @param str1 The string slice
/// @param str2 The char*
/// @return true if the strings are identical, false otherwise
bool slice_eq_str(Slice str1, char* str2);

/// @brief check whether two string slices are identical, use strcmp for char*
/// @param str1 the first string slice to be compared
/// @param str2 the second string slice to be compared
/// @return true if the string slices are identical, false otherwise
bool slice_eq(Slice str1, Slice str2);

bool slice_newline_after(Slice str);
SliceWordAfterInfo slice_whitespace_word_after(Slice str);
Slice slice_word_after(Slice str);

void slice_print(Slice str);

/// @brief Get the full path of a directory from arg0 and the desired subdir
/// @param argv0 argv[0]
/// @param sub_dir The subdirectory
/// @return The result of malloc, holding the desired partial arg0 + subdir
char* dir_path(char* argv0, char* sub_dir);

/// @brief Read the file contents at the given path and return it as a char*
/// @param dir Directory to search from
/// @param path Path to the file
/// @return char* of contents, which the caller must free
char* str_from_dir_and_path(char* dir, char* path);

/// @brief Read the file contents at the given path and return it as a slice
/// @param path Path to the file
/// @return String slice of contents, ending in \0, which the caller must free
Slice slice_from_path(char* path);

/// @brief Use a variable number of file paths to return their merged contents
/// @param paths Variable array of string slice file paths
/// @return A string slice of merged file contents, which the caller must free
Slice slice_from_paths(Slice* paths);
