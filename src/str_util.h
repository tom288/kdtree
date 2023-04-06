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
/// @param str1 the first string to be compared
/// @param str2 the second string to be compared
/// @return true if the strings are identical, false otherwise
bool string_identical(string_slice str1, string_slice str2);

// ---

/// @brief returns a smaller slice, the start of the slice moves forward to skip any occurrence of the provided characters
/// @param str1 the first string to be compared
/// @param str2 the second string to be compared
/// @return true if the strings are identical, false otherwise
string_slice string_skipChars(string_slice str, char* string_skipChars);

string_slice string_skipWhitespace(string_slice str);
string_slice string_skipUntilWhitespace(string_slice str, uint32_t startIndex);
string_slice string_after_expected(string_slice str, char* expected);

// ---

/// @brief skips a line break
/// @param str the string to be processed
/// @param startIndex the index where the comparison begins
/// @return the index of the first character after the line break
uint32_t string_skipLineBreak(char* str, uint32_t startIndex);
