#pragma once
#include "slice.h"
#include <cglm/cglm.h>

#define ORIENTATION_AXIS 1
#define ORIENTATION_INDEX 1 << 1
#define ORIENTATION_SQUARE 1 << 2
#define ORIENTATION_ABSOLUTE 1 << 3

typedef struct Replacement {
    // world.txt --> | top 50 room garden

    uint8_t orientation;
    // orientation bit mask
    // 0, 1 - x, y
    // 0, 1 - up/left, down/right
    // 0, 1 - square
    // 0, 1 - absolute
    // never 11xy

    float splitDecimal;
    size_t types_indices[2]; // two item pointer array
} Replacement;

typedef struct NodeType {
    vec3 col;
    Replacement* replacements; // Replacement array
    Slice typeName; // for use only when reading world.txt
} NodeType;

NodeType* rules_read();

void rules_print(NodeType* self);
void rules_print_node_type(NodeType self, size_t indent, NodeType* types);
void rules_print_replacements(Replacement* replacements, size_t indent, NodeType* types);
void rules_print_replacement(Replacement self, size_t indent, NodeType* types);
bool rules_orientation_ok(uint8_t orientation);
