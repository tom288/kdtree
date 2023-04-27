extern int WIP;
#include "kdtree.h"
//#include "queue.h"
#include "str_util.h"
#include <cglm/cglm.h>
#include <stb_ds.h>

typedef struct Replacement {
    // world.txt --> | top 50 room garden
    bool orientation; // top (true), left (false)
    uint8_t splitPercent;
    uint32_t types[2];
} Replacement;

typedef struct NodeType {
    uint32_t type;
    vec3* col;
    Replacement** replacements; // Replacement pointer array
    string_slice typeName; // for use only when reading world.txt
} NodeType;

string_slice read_file_into_buffer(char* path)
{
    FILE* const file = fopen(path, "rb");
    string_slice fail_slice;
    fail_slice.first = NULL;
    fail_slice.firstAfter = NULL;
    if (!file) {
        fprintf(stderr, "Failed to fopen ");
        perror(path);
        return fail_slice;
    }
    fseek(file, 0L, SEEK_END);
    const long file_size = ftell(file);
    rewind(file);
    char* const buffer = calloc(1, file_size + 1);
    if (fread(buffer, file_size, 1, file) != 1)
    {
        fclose(file);
        free(buffer);
        fprintf(stderr, "Failed to fread for %s\n", path);
        return fail_slice;
    }
    fclose(file);
    string_slice result;
    result.first = buffer;
    result.firstAfter = buffer + file_size;
    return result;
}

// reads and returns up to 2^32 node types from world.txt
// they are delimited by an empty line
// each node type in world.txt looks like this
//
// world
// 0 80 0
// | top 50 room garden
// | top 50 garden garden
NodeType* readRules()
{
    string_slice const file_prime = read_file_into_buffer("world.txt"); // cleaned up at the end of this function
    string_slice word = file_prime;
    word.first = strcmp(string_substr(word, 0, 19), "code starts here:\n\n") ? 19 :
        strcmp(string_substr(word, 0, 24), "---\n\ncode starts here") ? 24 :
        string_skipUntilThenSkip(word, "\n---\n\ncode starts here:\n\n", 0);
    word.firstAfter = word.first;
    NodeType* types = NULL; // nodeType array
    bool ok = true;

    while (ok) { // until end of file
        NodeType* this_node = arraddnptr(types, 1); // "last" refers to a new uninitialized node this adds to "types"

        // read typename
        word = string_wordAfter(word);
        for (uint32_t nodeIndex = 0; nodeIndex < arrlenu(types); nodeIndex++)
            if (string_identical(types[nodeIndex].typeName, string_wordAfter(word))) {
                this_node->type = nodeIndex;
                this_node->typeName = word;
            }

        // read col
        for (uint8_t n = 0; n < 3; n++) {
            vec3 col;
            for (uint8_t channel = 0; channel < 3; channel++) {
                word = string_wordAfter(word);
                col[channel] = strtof(word.first, word.firstAfter) / 100.0f;
            }
            this_node->col = col;
        }

        uint32_t replacementCount = 0;
        Replacement* this_replacement;
        while (word.first == '|') // until '|' keep reading replacement rule lines
        {
            this_replacement = this_node->replacements[replacementCount];

            // read orientation
            word = string_wordAfter(word);
            char* left_str = "left";
            string_slice left;
            left.first = left_str;
            left.firstAfter = left_str[4];
            this_replacement->orientation = string_identical(word, left);
            free(left_str);

            // read proportion
            word = string_wordAfter(word);
            this_replacement->splitPercent = strtof(word.first, word.firstAfter);

            // read two typenames
            string_wordAfter(word);
            for (uint8_t n = 0; n < 2; n++)
                for (uint32_t nodeIndex = 0; nodeIndex < arrlenu(types); nodeIndex++)
                    if (string_identical(this_node->typeName, word))
                        this_replacement->types[n] = nodeIndex;

            replacementCount++;
        }

        string_slice next = string_wordAfter(word);
        if (next.first == next.firstAfter) ok = false;
    };

    return types;
}

size_t rotate_left(size_t n, size_t dist) {
    return (n << dist) | (n >> (sizeof(size_t) * 8 - dist));
}

size_t rotate_left_quarter(size_t n, size_t count) {
    return rotate_left(n, count * sizeof(size_t) * 2);
}

typedef union {
    float from;
    size_t to;
} float_to_size;

// xor((int32)X, (int32)Y >> 8, (int32)width >> 16, (int32)height >> 24)
size_t sampleRandom(Node sample) { // uint64_t
    float_to_size x = {.from = sample.min_corner[0]};
    float_to_size y = {.from = sample.min_corner[1]};
    float_to_size width = {.from = sample.size[0]};
    float_to_size height = {.from = sample.size[1]};
    return x.to ^ rotate_left_quarter(y.to, 1) ^ rotate_left_quarter(width.to, 2) ^ rotate_left_quarter(height.to, 3);
}
