#pragma once
#include "str_util.h"
#include <cglm/cglm.h>

typedef struct Replacement {
    // world.txt --> | top 50 room garden
    bool orientation; // top (true), left (false)
    uint16_t splitPercent; // needs to be more than 8 bit to be examinable in memory properly
    struct NodeType* types[2]; // two item pointer array
} Replacement;

typedef struct NodeType {
    vec3 col;
    Replacement* replacements; // Replacement array
    string_slice typeName; // for use only when reading world.txt
} NodeType;

NodeType* readRules();

void rules_print(NodeType* self);
void rules_printNodeType(NodeType self, uint8_t indent);
void rules_printReplacements(Replacement* self, uint8_t indent);
void rules_printReplacement(Replacement self, uint8_t indent);
