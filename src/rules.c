#define __USE_MINGW_ANSI_STDIO 1 // Make MinGW printf support size_t with %zu
#include "rules.h"
#include "kdtree.h"
#include "utility.h"
#include "str_util.h"
#include "misc.h"
#include <cglm/cglm.h>
#include <stb_ds.h>

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
    NodeType* types = NULL; // NodeType array

    while (!string_empty(string_wordAfter(word))) { // until end of file
        NodeType* this_node = arraddnptr(types, 1); // new unset node pointer, added to "types"

        // Read typename
        word = string_wordAfter(word);
        this_node->typeName = word; // for debugging

        // Read col and divide it from 0-100 to 0-1
        vec3 col;
        for (uint8_t channel = 0; channel < 3; ++channel) {
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
            char* orientations[] = {"left", "up", "right", "down", "square_upLeft", "square_downRight"};
            for (size_t i = 0; i < sizeof(orientations) / sizeof(*orientations); ++i)
            {
                if (string_identical_str(word, orientations[i]))
                {
                    this_replacement->orientation = i;
                    break;
                }
            }

            // read split percent
            word = string_wordAfter(word);
            string_slice unit = word;
            this_replacement->splitDecimal = strtof(word.first, &unit.first) / 100.0f;
            char* const unit_words[] = { "cm", "m", "mm", "km", "um" };
            const float cm_to_unit[] = { 1.0f, 100.0f, 0.1f, 100000.0f, 0.0001f };

            for (uint8_t i = 0; i < sizeof(unit_words) / sizeof(*unit_words); ++i)
            {
                if (string_identical_str(unit, unit_words[i]))
                {
                    this_replacement->orientation |= ORIENTATION_ABSOLUTE;
                    this_replacement->splitDecimal *= cm_to_unit[i];
                    break;
                }
            }

            // read two typenames
            for (uint8_t n = 0; n < 2; ++n) {
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

        printf("\n");
        rules_print(types);
    };

    return types;
}

void rules_print(NodeType* self) {
    printf("rules:\n");
    for (size_t i = 0; i < arrlenu(self); ++i) {
        rules_NodeType_print(self[i], 1, self);
    }
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
    for (uint8_t i = 0; i < 3; ++i) {
        printf("%d", (int)(self.col[i] * 100.0f));
        printf(" ");
    }
    printf("\n"); ind(in); printf("rep: ");
    if (misc_notBad(self.replacements))
    {
        printf("\n");
        rules_replacements_print(self.replacements, in, types);
    }
}

void rules_replacements_print(Replacement* replacements, uint8_t indent, NodeType* types) {
    for (size_t i = 0; i < arrlenu(replacements); ++i) {
        rules_Replacement_print(replacements[i], indent, types);
    }
}

void rules_Replacement_print(Replacement self, uint8_t indent, NodeType* types) {
    uint8_t in = indent + 1;
    ind(in); printf("orientation: %d\n", (int)self.orientation);
    ind(in); printf("split: %f\n", self.splitDecimal);
    for (uint8_t i = 0; i < 2; ++i) {
        ind(in); printf("child type name: ");
        if (misc_notBad(&types[self.types_indices[i]].typeName)) {
            string_print(types[self.types_indices[i]].typeName);
        }
        printf("\n");
    }
}

bool rules_replacement_orientation_notBad(uint8_t orientation) {
    return !(orientation & ORIENTATION_SQUARE
          && orientation & ORIENTATION_ABSOLUTE);
}
