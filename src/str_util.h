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

/// @brief returns a shorter slice, the start of the slice moves forward to the first character that's not in skipChars
/// @param str the slice to be shortened
/// @param skipChars shorten the slice until a character not in these characters is reached
/// @return the shorter slice, the start of the slice moves forward to the first character that's not in skipChars
string_slice string_skipChars(string_slice str, char* skipChars);

/// @brief returns a shorter slice, the start of the slice moves forward to the first non whitespace character
/// @param str the slice to be shortened
/// @return the shorter slice, the start of the slice moves forward to the first non whitespace character
string_slice string_skipWhitespace(string_slice str);

/// @brief returns a shorter slice, the start of the slice moves forward to the first character that's whitespace
/// @param str the slice to be shortened
/// @return the shorter slice, the start of the slice moves forward to the first character that's whitespace
string_slice string_skipNonWhitespace(string_slice str);

// ---

/// @brief returns a shorter slice, the start of the slice moves forward to the first character after the expected text
/// text before the expected text is also not included
/// @param str the slice to be shortened
/// @return the shorter slice, the start of the slice moves forward to the first character after the expected text
string_slice string_after_expected(string_slice str, char* expected);

/// @brief skips a line break
/// @param str the string to be processed
string_slice string_skipLineBreak(string_slice str);
