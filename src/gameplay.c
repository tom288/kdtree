#include <cglm/cglm.h>

typedef struct Entity {
    vec3 pos; // x, y, height (0, 0, 0 = center and on ground, negatives for underground)
    float angle; // 0 = upwards
} Entity;

typedef struct Entity2 {
    vec3 vel;
    float scale; // 0, 1, 2 = point, normal, double, etc...
    uint8_t type; // 0, 1, 2, 3 = fixed size, laser, double ended laser, expandable
    bool solid; // false if I can move through things
} Entity2;

typedef struct Player_stats {
    struct Entity ent;
    struct Entity2 ent2;
    uint16_t health; // 0, 100 = defeated, starting, also can exceed 100
    int8_t health_changePerSec; // 0, positive, negative = normal, regenerating, decaying
    uint8_t speed; // 0, 100 = turret, normal
    uint8_t acceleration; // 0, 100 = super ice,
    int16_t armour_lower; // -1, 0, 100 = non-existant, none, normal
    int16_t armour_mid; // -1, 0, 100 = non-existant, none, normal
    int16_t armour_upper; // -1, 0, 100 = non-existant, none, normal
    char* name; // for flavour, hello my name is ...
} Player;

typedef struct Item_inHand {
    uint16_t type;
} Item_inHand;

typedef struct Bull { // includes bullets, lasers, grenades, and the such
    uint16_t type;
} Bull;

void gameplay_player_lookAt(Entity* self, Entity target) {
    printf("implement gameplay_player_lookAt(vec3)\n");
}

gameplay_playerCount = 5;

Player gameplay_player(uint8_t choice) {
    Player def = (Player) {
        .ent = (Entity) {.pos = {0, 0, 0}, .angle = 0},
        .ent2 = (Entity2) {.vel = {0, 0, 0}, .scale = 1, .type = 0, .scale = 1},
        .health = 100, .health_changePerSec = 0,
        .speed = 100, .armour_lower = 100, .armour_mid = 100, .armour_upper = 50,
        .name = "Unremarkable Ralph"};
    switch (choice)
    {
        case 0: return def; break;

        case 1:
            def.armour_lower = -1;
            def.armour_upper = -1;
            def.armour_mid = 50;
            def.health_changePerSec = 1;
            def.
            return def;
    }
}
