#include <stdint.h>
#include <stdbool.h>

/// @brief yields the length of a string
/// @param str string to be measured
/// @return length of the string
uint32_t string_len(char* str);

/// @brief yields a substring of a string
/// @param str the string to be trimmed
/// @param startIndex the index where the substring begins
/// @param len desired length of the substring
/// @return the trimmed string
char* string_substr(char* str, uint32_t startIndex, uint32_t len);

/// @brief checks if two strings are identical
/// @param str1 the first string to be compared
/// @param str2 the second string to be compared
/// @return true if the strings are identical, false otherwise
bool string_identical(char* str1, char* str2);

// ---

/// @brief start and end indices of substring
/// last = (firstAfter - 1)
/// (firstAfter - first) = length
/// HelloWorld
/// sub: elloWo
/// first: 1
/// last: 6
/// firstAfter: 7
typedef struct string_slice {
    uint32_t index_first;
    uint32_t index_firstAfter;
} string_slice;

string_slice string_skipChars(char* str, char* string_skipChars, uint32_t startIndex);
string_slice string_skipWhitespace(char* str, uint32_t startIndex);
string_slice string_skipUntilWhitespace(char* str, uint32_t startIndex);
string_slice string_skipUntilThenSkipWhitespace(char* str, char* expected, uint32_t startIndex);

// ---

/// @brief skips a line break
/// @param str the string to be processed
/// @param startIndex the index where the comparison begins
/// @return the index of the first character after the line break
uint32_t string_skipLineBreak(char* str, uint32_t startIndex);
