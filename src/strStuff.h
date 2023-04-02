#include <stdint.h>
#include <stdbool.h>

uint32_t strStuff_len(char* str);
char* strStuff_trim(char* str, uint32_t startIndex, uint32_t len);
bool strStuff_unexpected(char* str, char* expected, uint32_t startIndex);
bool strStuff_identical(char* str1, char* str2);

// ---

uint32_t strStuff_skipUntilThenSkipWhitespace(char* str, char* expected, uint32_t startIndex); // used enough to justify?
uint32_t strStuff_skipUntilWhitespace(char* str, uint32_t startIndex);
uint32_t strStuff_skipLineBreak(char* str, uint32_t startIndex);

// ---

uint32_t strStuff_skipChars(char* str, uint32_t startIndex);
uint32_t strStuff_skipWhitespace(char* str, uint32_t startIndex);
