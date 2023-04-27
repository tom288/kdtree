#pragma once
#include <cglm/cglm.h>
#include "str_util.h"

typedef struct Replacement {
    // world.txt --> | top 50 room garden
    bool orientation; // top (true), left (false)
    uint8_t splitPercent;
    uint32_t types[2];
} Replacement;

typedef struct NodeType {
    uint32_t type;
    vec3 col;
    Replacement* replacements; // Replacement pointer array
    string_slice typeName; // for use only when reading world.txt
} NodeType;

NodeType* readRules();
