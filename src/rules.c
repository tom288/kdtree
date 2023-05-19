#define __USE_MINGW_ANSI_STDIO 1 // Make MinGW printf support size_t with %zu
#include "rules.h"
#include "kdtree.h"
#include "utility.h"
#include "str_util.h"
#include "misc.h"
#include <cglm/cglm.h>
#include <stb_ds.h>

typedef struct ImportLines {
    string_slice* filenames;
    string_slice word_prev;
} ImportLines;

ImportLines read_importLines() {
    string_slice* filenames = NULL;
    string_slice* this_filename = arraddnptr(filenames, 1);
    char* this_filename2 = "src/world/world.txt";
    *this_filename = (string_slice) {
        .first = this_filename2, .firstAfter = this_filename2 + strlen(this_filename2)};
    string_slice file_main = read_file_into_buffer(this_filename2);
    string_slice word = string_wordAfter(word);
    string_slice word_prev;
    while (string_identical_str(word, ">")) {
        word = string_wordAfter(word);
        file_main = *arraddnptr(filenames, 1);

        // make a string out of a string slice hack
        char prev = *word.firstAfter;
        *word.firstAfter = '\0'; // string complete
        *this_filename = word;
        *word.firstAfter = prev; // string no longer usable, no clean up needed

        word_prev = word;
        word = string_wordAfter(word);
    }
    return (ImportLines) {filenames: filenames, word_prev: word_prev};
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
    ImportLines importLines = read_importLines();
    string_slice* file = merge_files(importLines.filenames); // todo: free(file)
    string_slice word = importLines.word_prev;
    word.firstAfter = word.first;
    const string_slice start = word;

    // read typenames and their indices
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
    while (!string_empty(string_wordAfter(word))) // read a file
    {
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
                for (size_t i = 0; i < arrlenu(type_names); ++i)
                {
                    if (string_identical(type_names[i], word)) {
                        this_replacement->types_indices[n] = i;
                        break;
                    }
                }
            }
        }
    }

    rules_print(types); // todo: debug info, remove me

    return types;
}

void rules_print(NodeType* self) {
    printf("\nrules:\n");
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
