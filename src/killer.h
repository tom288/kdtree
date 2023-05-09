#pragma once

#include <stb_ds.h>
#include <GL/glew.h>

typedef void (*kill_fn)(void*);

typedef struct {
    kill_fn* kill_fns;
    void** kill_args;
} Killer;

/// @brief Return a new killer
/// @return The new killer
Killer killer_init();

/// @brief Assert the input is true, otherwise call killer_kill
/// @param killer Killer to assert for
/// @param b Value to assert is true
/// @return Value of b
int killer_assert(Killer* killer, GLboolean b);

/// @brief Add a new kill target to the killer
/// @param killer Killer to update
/// @param fn Function to call when killing
/// @param arg Argument to pass when killing
void killer_target(Killer* killer, kill_fn fn, void* arg);

/// @brief Kill all recorded targets followed by the killer
/// @param killer Killer
/// @return 0
int killer_kill(Killer* killer);
