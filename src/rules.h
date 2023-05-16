#pragma once
#include "str_util.h"
#include <cglm/cglm.h>

typedef struct Replacement {
    // world.txt --> | top 50 room garden

    uint8_t orientation;
    // orientation bit mask
    // 0, 1 - x, y
    // 0, 1 - up/left, down/right
    // 0, 1 - square
    // 0, 1 - absolute
    // never 11xy

    bool percentMeters;
    uint8_t splitPercent;
    float splitMeters;
    size_t types_indices[2]; // two item pointer array
} Replacement;

typedef struct NodeType {
    vec3 col;
    Replacement* replacements; // Replacement array
    string_slice typeName; // for use only when reading world.txt
} NodeType;

NodeType* readRules();

void rules_print(NodeType* self);
void rules_NodeType_print(NodeType self, uint8_t indent, NodeType* types);
void rules_replacements_print(Replacement* replacements, uint8_t indent, NodeType* types);
void rules_Replacement_print(Replacement self, uint8_t indent, NodeType* types);
bool rules_replacement_orientation_notBad(uint8_t orientation);
