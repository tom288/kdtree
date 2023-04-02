#include <stdint.h>
#include <stdbool.h>

uint32_t string_len(char* str);
char* string_trim(char* str, uint32_t startIndex, uint32_t len);
bool string_unexpected(char* str, char* expected, uint32_t startIndex);
bool string_identical(char* str1, char* str2);

// ---

uint32_t string_skipUntilThenSkipWhitespace(char* str, char* expected, uint32_t startIndex); // used enough to justify?
uint32_t string_skipUntilWhitespace(char* str, uint32_t startIndex);
uint32_t string_skipLineBreak(char* str, uint32_t startIndex);

// ---

uint32_t string_skipChars(char* str, uint32_t startIndex);
uint32_t string_skipWhitespace(char* str, uint32_t startIndex);
