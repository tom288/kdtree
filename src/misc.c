#include <cglm/cglm.h>

void misc_ind(size_t indent)
{
    for (size_t i = 0; i < indent; ++i) printf("  ");
}

bool misc_not_null(void* z)
{
    if (z != NULL) return true;
    printf("misc_not_null called on NULL\n");
    return false;
}
