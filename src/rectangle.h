#pragma once

#include "shader.h"
#include "graph.h"

/// @brief Create a centered rectangle with edges -0.5 to 0.5
/// @param shader Shader to use for attribute position names
/// @return Graph holding rectangle data
Graph rectangle_init(Shader* shader);
