#include "killer.h"

Killer killer_init()
{
    return (Killer) {
        .kill_fns = NULL,
        .kill_args = NULL,
    };
}

int killer_assert(Killer killer, GLboolean b)
{
    if (!b) killer_kill(killer);
    return b;
}

void killer_target(Killer killer, kill_fn fn, void* arg)
{
    arrput(killer.kill_fns, (kill_fn)fn);
    arrput(killer.kill_args, arg);
}

int killer_kill(Killer killer)
{
    for (size_t i = 0; i < arrlenu(killer.kill_fns); ++i)
    {
        (killer.kill_fns[i])(killer.kill_args[i]);
    }
    arrfree(killer.kill_fns);
    arrfree(killer.kill_args);
    return 0;
}
