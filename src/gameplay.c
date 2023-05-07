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

// ---

typedef struct Player_stats {
    struct Entity ent;
    struct Entity2 ent2;
    uint16_t health; // 0, 100 = defeated, starting, also can exceed 100
    int8_t health_changePerSec; // 0, positive, negative = normal, regenerating, decaying
    uint8_t friction; // 0, 100 = superglue, normal
    uint8_t acceleration; // 0, 100 = no strength, normal
    int16_t armour_lower; // -1, 0, 100 = non-existant, none, normal
    int16_t armour_mid; // -1, 0, 100 = non-existant, none, normal
    int16_t armour_upper; // -1, 0, 100 = non-existant, none, normal
    uint16_t strength; // 0, 100 = slowed to a stop carrying anything, normal strength
    char* name; // for flavour, hello my name is ...
} Player_stats;

gameplay_player_stats_count = 5;

Player_stats gameplay_player_stats(uint8_t choice) {
    Player_stats def = (Player_stats) {
        .ent = (Entity) {.pos = {0, 0, 0}, .angle = 0},
        .ent2 = (Entity2) {.vel = {0, 0, 0}, .scale = 1, .type = 0, .scale = 1},
        .health = 100, .health_changePerSec = 0,
        .friction = 100, .acceleration = 100,
        .armour_lower = 100, .armour_mid = 100, .armour_upper = 50,
        .strength = 100, .name = "All-Rounder Alex"};
    switch (choice)
    {
        case 0: return def; break;

        case 1:
            def.name = "Rodger the Robot";
            def.health_changePerSec = 1;
            def.acceleration = 50;
            def.friction = 20;
            def.armour_lower = -1;
            def.armour_upper = -1;
            def.armour_mid = 50;

        case 2:
            def.name = "Barry the Bum";
            def.health = 80;
            def.acceleration = 80;
            def.armour_lower = 80;
            def.armour_mid = 80;
            def.armour_upper = 40;
            def.strength = 80;

        case 3:
            def.name = "Bulky Bob";
            def.health = 120;
            def.acceleration = 80;
            def.friction = 80;
            def.armour_lower = 120;
            def.armour_mid = 120;
            def.armour_upper = 60;
            def.strength = 200;

        case 4:
            def.name = "Slick Sam";
            def.health = 80;
            def.acceleration = 150;
            def.armour_lower = 80;
            def.armour_mid = 80;
            def.armour_upper = 40;
            def.strength = 50;

        return def;
    }
}

// ---

typedef struct Player {
    struct Entity ent;
    struct Entity2 ent2;
    uint16_t health; // 0, 100 = defeated, starting, also can exceed 100
    bool onGround;
} Player;

Player gameplay_player(Player_stats stats) {
    return (Player) {
        .ent = (Entity) {.pos = {0, 0, 0}, .angle = 0},
        .ent2 = (Entity2) {.vel = {0, 0, 0}, .scale = 1, .type = 0, .scale = 1},
        .health = stats.health};
}

void gameplay_player_lookAt(Entity* self, Entity target) {
    printf("implement gameplay_player_lookAt(vec3)\n");
}

Player gameplay_player_physStep(Player self, Player_stats stats) {
    if (self.onGround) {
        for (uint8_t n = 0; n < 3; n++)
            self.ent.pos[n] = (self.ent.pos[n] + self.ent2.vel[n]) * (stats.friction / 100.0);
    }
    self.ent2.vel[2] -= 0.05;
    self.ent.pos[2] -= self.ent2.vel[2];
    if (self.ent.pos[2] < 0) self.ent.pos[2] = 0;
    return self;
}

// ---

typedef struct Bull { // includes bullets, lasers, grenades, and the such
    uint16_t type;
} Bull;

// ---

typedef struct Gameplay_item_stats {
    uint16_t type;
} Item_inHand;
