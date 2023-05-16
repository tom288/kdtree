#include <cglm/cglm.h>

void ind(uint8_t indent) {
    for (uint8_t n = 0; n < indent; n++) printf("  ");
}

bool misc_notBad(void* anything) { // for debugging only
    if (anything != NULL) return true;
    printf("null, could be empty array\n");
    return false;
}
