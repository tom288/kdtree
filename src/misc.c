#include <cglm/cglm.h>

void misc_ind(size_t indent)
{
    for (size_t n = 0; n < indent; n++) printf("  ");
}

bool misc_not_null(void* z)
{
    if (z != NULL) return true;
    printf("misc_not_null called on NULL\n");
    return false;
}
