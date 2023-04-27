#pragma once
#include "str_util.h"
#include <cglm/cglm.h>

typedef struct Replacement {
    // world.txt --> | top 50 room garden
    bool orientation; // top (true), left (false)
    uint8_t splitPercent;
    struct NodeType* types[2]; // two item pointer array
} Replacement;

typedef struct NodeType {
    vec3 col;
    Replacement* replacements; // Replacement array
    string_slice typeName; // for use only when reading world.txt
} NodeType;

NodeType* readRules();
