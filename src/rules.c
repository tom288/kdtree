#define __USE_MINGW_ANSI_STDIO 1 // Make MinGW printf support size_t with %zu
#include "rules.h"
#include "kdtree.h"
#include "utility.h"
#include "slice.h"
#include "misc.h"
#include <cglm/cglm.h>
#include <stb_ds.h>

Slice rules_import_and_merge()
{
    Slice world = slice_from_path("world.txt");
    return world; // todo: remove this line

    Slice word = { .first = world.first, .firstAfter = world.first };
    word = slice_word_after(word);
    Slice* paths = NULL;

    while (*word.first)
    {
        arrput(paths, word);
        word = slice_word_after(word);
    }

    // Include the standard libraries
    arrput(paths, slice_from_str("standard.txt"));
    arrput(paths, slice_from_str("standard_cols.txt"));
    arrput(paths, slice_from_str("standard_hv.txt"));
    arrput(paths, slice_from_str("standard_recurse.txt"));

    Slice big = slice_from_paths(paths);
    arrfree(paths);
    free(world.first);
    return big;
}

Slice rules_append_two_typenames(Slice word, Slice* type_names,
    Replacement* this_replacement, NodeType* types) {
    // read and record two typenames, along with the parameters to pass to them
    for (size_t type_index = 0; type_index < 2; ++type_index)
    {
        word = slice_word_after(word);
        bool found = false;
        for (size_t i = 0; i < arrlenu(type_names); ++i)
        {
            if (slice_eq(type_names[i], word))
            {
                this_replacement->types_indices[type_index] = i;
                found = true;
                break;
            }
        }
        if (found) {
            word.firstAfter = word.first;
            return word;
        }
        else {
            arrpop(types);
            printf("Failed to find type name '");
            slice_print(word);
            printf("'\n");
        }
    }
    return (Slice) { NULL };
}

// reads and returns types from world.txt
// they are delimited by an empty line
// each node type in world.txt looks like this
//
// world
// 0 80 0
// | up 50 room garden
// | up 50 garden garden
NodeType* rules_read()
{
    // Slice file = rules_import_and_merge();
    Slice file = slice_from_path("world2.txt");
    Slice word = { file.first, file.first };

    // Record typenames and their indices
    Slice* type_names = NULL;
    for (size_t i = 0; slice_len(slice_word_after(word)); ++i)
    {
        // Record typename and index
        word = slice_word_after(word);
        arrput(type_names, word);
        // Skip the rest of the block
        while (slice_whitespace_word_after(word).tabsAfterLineBreak != 0) {
            word = slice_word_after(word);
            while (!slice_whitespace_word_after(word).lineBreak) {
                word = slice_word_after(word);
            }
        }
    }
    word = (Slice) { .first = file.first, .firstAfter = file.first };

    NodeType* types = NULL; // NodeType array
    while (slice_len(slice_word_after(word))) // read a file
    {
        NodeType* this_node = arraddnptr(types, 1); // new unset node pointer, added to "types"

        // Read typename
        word = slice_word_after(word);
        this_node->typeName = word; // for debugging

        // Read col and cast it to a uint8, 0 to 255
        for (size_t channel = 0; channel < 3; ++channel)
        {
            word = slice_word_after(word);
            this_node->col[channel] = (uint8_t)glm_clamp(
                strtof(word.first, NULL), 0.0f, 255.0f
            );
        }

        this_node->replacements = NULL;
        if (slice_whitespace_word_after(word).tabsAfterLineBreak == 1)
        {
            while (true) // until '|' keep reading replacement rule lines
            {
                Slice word_ahead = slice_word_after(word);
                if (!slice_eq_str(word_ahead, "|")) break;
                word = word_ahead;

                // refers to a new uninitialized replacement in this_node
                Replacement* this_replacement = arraddnptr(this_node->replacements, 1);

                // read orientation
                // 0, 1 - x, y
                // 0, 1 - up/left, down/right
                // 0, 1 - square
                // 0, 1 - absolute
                this_replacement->orientation = 255;
                word = slice_word_after(word);
                char* orientations[] = {"left", "up", "right", "down", "square_upLeft", "square_downRight"};
                for (size_t i = 0; i < sizeof(orientations) / sizeof(*orientations); ++i)
                {
                    if (slice_eq_str(word, orientations[i]))
                    {
                        this_replacement->orientation = i;
                        break;
                    }
                }
                if (this_replacement->orientation == 255) {
                    error("no such orientation");
                }

                // read split percent
                word = slice_word_after(word);
                Slice unit = word;
                this_replacement->splitDecimal = strtof(word.first, &unit.first) / 100.0f;
                char* const unit_words[] = { "cm", "m", "mm", "km", "um" };
                const float cm_to_unit[] = { 1.0f, 100.0f, 0.1f, 100000.0f, 0.0001f };

                for (size_t i = 0; i < sizeof(unit_words) / sizeof(*unit_words); ++i)
                {
                    if (slice_eq_str(unit, unit_words[i]))
                    {
                        this_replacement->orientation |= ORIENTATION_ABSOLUTE;
                        this_replacement->splitDecimal *= cm_to_unit[i];
                        break;
                    }
                }

                word = rules_append_two_typenames(word, type_names, this_replacement, types);
                if (word.first == NULL) return NULL;
            }
        }
    }
    free(file.first); // can no longer use typenames
    return types;
}

void rules_print(NodeType* self)
{
    printf("\nrules:\n");
    for (size_t i = 0; i < arrlenu(self); ++i)
    {
        rules_print_node_type(self[i], 1, self);
    }
}

void rules_print_node_type(NodeType self, size_t indent, NodeType* types)
{
    misc_ind(indent++);
    printf("NodeType:\n");

    misc_ind(indent);
    printf("type name: ");
    if (misc_not_null(&(self.typeName)))
    {
        slice_print(self.typeName);
        printf("\n");
    }

    misc_ind(indent);
    printf("col:");
    for (size_t i = 0; i < 3; ++i)
    {
        printf(" ");
        printf("%d", (int)(self.col[i] * 100.0f));
    }
    printf("\n");

    misc_ind(indent);
    printf("rep: ");
    if (misc_not_null(self.replacements))
    {
        printf("\n");
        rules_print_replacements(self.replacements, indent, types);
    }
}

void rules_print_replacements(Replacement* replacements, size_t indent, NodeType* types)
{
    for (size_t i = 0; i < arrlenu(replacements); ++i)
    {
        rules_print_replacement(replacements[i], indent, types);
    }
}

void rules_print_replacement(Replacement self, size_t indent, NodeType* types)
{
    misc_ind(++indent);
    printf("orientation: %d\n", (int)self.orientation);

    misc_ind(indent);
    printf("split: %f\n", self.splitDecimal);

    for (size_t i = 0; i < 2; ++i)
    {
        misc_ind(indent);
        printf("child type name: ");
        if (misc_not_null(&types[self.types_indices[i]].typeName))
        {
            slice_print(types[self.types_indices[i]].typeName);
        }
        printf("\n");
    }
}

bool rules_orientation_ok(uint8_t orientation)
{
    return !(orientation & ORIENTATION_SQUARE
          && orientation & ORIENTATION_ABSOLUTE);
}
