extern int WIP;
#include "rules.h"
#include "kdtree.h"
#include "utility.h"
#include "str_util.h"
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
    string_slice const file_prime = read_file_into_buffer("src/world2.txt"); // cleaned up at end
    string_slice word = string_after_expected(file_prime, "---");
    for (uint8_t n = 0; n < 2; n++) // to prevent problems with \r\n on many systems
        word = string_after_expected(word, "\n");
    word = string_after_expected(word, "code starts here:");
    word = string_after_expected(word, "\n");
    word.firstAfter = word.first;
    NodeType* types = NULL; // nodeType array
    bool ok = true;

    // TEMP CODE
    for (uint8_t n = 0; n < 50; n++)
        printf("\n");

    while (ok) { // until end of file
        NodeType* this_node = arraddnptr(types, 1); // new unset node pointer, added to "types"

        // read typename
        word = string_wordAfter(word);
        if (string_empty(word)) {
            arrpop(types);
            return types;
        }
        this_node->typeName = word; // for debugging

        // read col
        vec3 col;
        for (uint8_t channel = 0; channel < 3; channel++) {
            word = string_wordAfter(word);
            if (string_empty(word)) {
                arrpop(types);
                return types;
            }
            col[channel] = strtof(word.first, NULL) / 100.0f;
        }
        glm_vec3_copy(col, this_node->col);

        this_node->replacements = NULL;
        while (true) // until '|' keep reading replacement rule lines
        {
            string_slice word_ahead = string_wordAfter(word);
            if ((*word_ahead.first != '|') || ((word_ahead.firstAfter - 1) != word_ahead.first)) break;
            word = word_ahead;

            // refers to a new uninitialized replacement in this_node
            Replacement* this_replacement = arraddnptr(this_node->replacements, 1);

            // read orientation
            word = string_wordAfter(word);
            if (string_empty(word)) return types;
            char* top_str = "top";
            string_slice top;
            top.first = top_str;
            top.firstAfter = &(top_str[3]);
            this_replacement->orientation = string_identical(word, top);

            // read split percent
            word = string_wordAfter(word);
            if (string_empty(word)) return types;
            this_replacement->splitPercent = strtof(word.first, NULL);

            // read two typenames
            for (uint8_t n = 0; n < 2; n++) {
                word = string_wordAfter(word);
                for (uint32_t nodeIndex = 0; nodeIndex < arrlenu(types); nodeIndex++)
                    if (string_identical(types[nodeIndex].typeName, word)) {
                        this_replacement->types_indices[n] = nodeIndex;
                        break;
                    }
            }
        }

        if (string_empty(string_wordAfter(word))) ok = false;
    };

    return types;
}

void ind(uint8_t indent) {
    for (uint8_t n = 0; n < indent; n++) printf("  ");
}

bool notBad(void* anything) {
    if (anything != NULL) return true;
    printf("null, could be empty array\n");
    return false;
}

void rules_print(NodeType* self) {
    printf("rules:\n");
    for (uint32_t n = 0; n < arrlenu(self); n++)
        rules_printNodeType(self[n], 1, self);
}

void rules_printNodeType(NodeType self, uint8_t indent, NodeType* types) {
    ind(indent); printf("NodeType:\n");
    uint8_t in = indent + 1;
    ind(in); printf("type name: ");
    if (notBad(&(self.typeName))) {
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
    if (notBad(self.replacements)) {
        printf("\n");
        rules_printReplacements(self.replacements, in, types);
    }
}

void rules_printReplacements(Replacement* replacements, uint8_t indent, NodeType* types) {
    for (uint32_t n = 0; n < arrlenu(replacements); n++)
        rules_printReplacement(replacements[n], indent, types);
}

void rules_printReplacement(Replacement self, uint8_t indent, NodeType* types) {
    uint8_t in = indent + 1;
    ind(in); printf("orientation: %s\n", self.orientation ? "true" : "false");
    ind(in); printf("split percent: %d\n", self.splitPercent);
    for (uint8_t n = 0; n < 2; n++) {
        ind(in); printf("child type name: ");
        if (notBad(&types[self.types_indices[n]].typeName))
            string_print(types[self.types_indices[n]].typeName);
        printf("\n");
    }
}
