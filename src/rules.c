#define __USE_MINGW_ANSI_STDIO 1 // Make MinGW printf support size_t with %zu
#include "rules.h"
#include "kdtree.h"
#include "utility.h"
#include "str_util.h"
#include "misc.h"
#include <cglm/cglm.h>
#include <stb_ds.h>

string_slice read_directory_into_buffer(char* path) {
    string_slice slice;
    slice.first = NULL;
    slice.firstAfter = NULL;
    for (uint32_t n = 0; n < 10; n++) {
        ;
    }
    return slice;
}

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
    string_slice const file = read_file_into_buffer("src/world/world.txt"); // todo: free(file)
    string_slice word = file;
    word.firstAfter = word.first;
    const string_slice start = word;

    // Record typenames and their indices
    string_slice* type_names = NULL;
    for (size_t i = 0; !string_empty(string_wordAfter(word)); ++i)
    {
        // Record typename and index
        word = string_wordAfter(word);
        arrput(type_names, word);
        // Skip R, G, B
        for (size_t i = 0; i < 3; ++i) word = string_wordAfter(word);
        // Skip replacements
        while (string_identical_str(string_wordAfter(word), "|"))
            for (size_t i = 0; i < 5; ++i) word = string_wordAfter(word);
    }

    word = start;
    NodeType* types = NULL; // nodeType array

    while (!string_empty(string_wordAfter(word))) { // until end of file
        NodeType* this_node = arraddnptr(types, 1); // new unset node pointer, added to "types"

        // read typename
        word = string_wordAfter(word);
        this_node->typeName = word; // for debugging

        // read col
        vec3 col;
        for (uint8_t channel = 0; channel < 3; channel++) {
            word = string_wordAfter(word);
            col[channel] = strtof(word.first, NULL) / 100.0f;
        }
        glm_vec3_copy(col, this_node->col);

        this_node->replacements = NULL;
        while (true) // until '|' keep reading replacement rule lines
        {
            string_slice word_ahead = string_wordAfter(word);
            if (!string_identical_str(word_ahead, "|")) break;
            word = word_ahead;

            // refers to a new uninitialized replacement in this_node
            Replacement* this_replacement = arraddnptr(this_node->replacements, 1);

            // read orientation
            // 0, 1 - x, y
            // 0, 1 - up/left, down/right
            // 0, 1 - square
            // 0, 1 - absolute
            word = string_wordAfter(word);
            char* arr1[] = {"left", "up", "right", "down", "square_upLeft", "square_downRight"};
            for (uint8_t n = 0; n < 6; n++)
                if (string_identical_str(word, arr1[n])) {
                    this_replacement->orientation = n;
                    break;
                }

            // read split percent
            word = string_wordAfter(word);
            this_replacement->percentMeters = false;
            string_slice unit = word;
            this_replacement->splitPercent = strtof(word.first, &unit.first);
            char* arr2[] = {"cm", "m", "mm", "km", "um"};
            for (uint8_t n = 0; n < 5; n++)
                if (string_identical_str(unit, arr2[n])) {
                    this_replacement->percentMeters = true;
                    this_replacement->orientation |= 8; // 0b1000
                    this_replacement->splitMeters = this_replacement->splitPercent *
                        (float[]){0.01f, 1.0f, 0.001f, 1000.0f, 0.000001f}[n];
                    break;
                }

            // read two typenames
            for (uint8_t n = 0; n < 2; n++) {
                word = string_wordAfter(word);
                bool found = false;
                for (size_t i = 0; i < arrlenu(type_names); ++i)
                {
                    if (string_identical(type_names[i], word))
                    {
                        this_replacement->types_indices[n] = i;
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    arrpop(types);
                    return types;
                }
            }
        }
    };

    return types;
}

void rules_print(NodeType* self) {
    printf("rules:\n");
    for (uint32_t n = 0; n < arrlenu(self); n++)
        rules_NodeType_print(self[n], 1, self);
}

void rules_NodeType_print(NodeType self, uint8_t indent, NodeType* types) {
    ind(indent); printf("NodeType:\n");
    uint8_t in = indent + 1;
    ind(in); printf("type name: ");
    if (misc_notBad(&(self.typeName))) {
        string_print(self.typeName);
        printf("\n");
    }
    ind(in); printf("col: ");
    for (uint8_t m = 0; m < 3; m++) {
        printf("%d", (int)(self.col[m] * 100));
        printf(" ");
    }
    printf("\n");
    ind(in); printf("rep: ");
    if (misc_notBad(self.replacements)) {
        printf("\n");
        rules_replacements_print(self.replacements, in, types);
    }
}

void rules_replacements_print(Replacement* replacements, uint8_t indent, NodeType* types) {
    for (uint32_t n = 0; n < arrlenu(replacements); n++)
       rules_Replacement_print(replacements[n], indent, types);
}

void rules_Replacement_print(Replacement self, uint8_t indent, NodeType* types) {
    uint8_t in = indent + 1;
    ind(in); printf("orientation: %d\n", (int)self.orientation);
    ind(in); printf("split percent: %d\n", self.splitPercent);
    ind(in); printf("split meters: %d\n", self.splitMeters);
    for (uint8_t n = 0; n < 2; n++) {
        ind(in); printf("child type name: ");
        if (misc_notBad(&types[self.types_indices[n]].typeName))
            string_print(types[self.types_indices[n]].typeName);
        printf("\n");
    }
}

bool rules_replacement_orientation_notBad(uint8_t orientation) {
    const uint8_t x_y = 0, upLeft_downRight = 1, square = 3, absolute = 4; // bit index in orientation
    if (((orientation >> 3) & 1) && ((orientation >> 4) & 1)) return false;
    return true;
}
