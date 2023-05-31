#include "gameplay.h"
#include <cglm/cglm.h>
#include <math.h>

Entity2 entity2() {
    return (Entity2) {
        .ent = {{0.0, 0.0, 0.0}, 0.0},
        .vel = {1.0, 1.0, 1.0},
        .scale = 1.0,
        .type = 0,
        .solid = false
    };
}

GameplayPlayerStats gameplay_player_stats(uint8_t choice) {
    GameplayPlayerStats def = (GameplayPlayerStats) {
        .ent2 = (Entity2) {.ent = {{0, 0, 0}, 0}, .vel = {0, 0, 0}, .scale = 1, .type = 3, .solid = true},
        .health = 100, .health_changePerSec = 0,
        .friction = 100, .acceleration = 100,
        .armour_lower = 100, .armour_mid = 100, .armour_upper = 50,
        .strength = 100, .name = "All-Rounder Alex"
    };
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
            break;

        case 2:
            def.name = "Barry the Bum";
            def.health = 80;
            def.acceleration = 80;
            def.armour_lower = 80;
            def.armour_mid = 80;
            def.armour_upper = 40;
            def.strength = 80;
            break;

        case 3:
            def.name = "Bulky Bob";
            def.health = 120;
            def.acceleration = 80;
            def.friction = 80;
            def.armour_lower = 120;
            def.armour_mid = 120;
            def.armour_upper = 60;
            def.strength = 200;
            break;

        case 4:
            def.name = "Slick Sam";
            def.health = 80;
            def.acceleration = 150;
            def.armour_lower = 80;
            def.armour_mid = 80;
            def.armour_upper = 40;
            def.strength = 50;
            break;

    }
    return def;
}

Entity3 entity3() {
    return (Entity3) {.ent2 = entity2(), .onGround = false};
}

const GameplayDrive gameplay_drive = {false, NULL, NULL, NULL};

GameplayDrive gameplay_drive_physStep(GameplayDrive d) {
    uint8_t speed_index = 3;
    if (d.accelerating == true) d.values[speed_index] += 0.1f;
    d.values[speed_index] *= 0.9f;
    return d;
}

// todo: what easing function ID
GameplayDrive gameplay_drive_physStep2(GameplayDrive d) {
    for (uint8_t n = 0; n < gameplay_drive_valuesCount; n++) {
        for (uint8_t m = 0; m < gameplay_drive_valuesCount; m++) {
            d.values[n] = gameplay_easing(0, d.values[m]) * d.weights[n][m] + d.biases[n][m];
        }
    }
    return d;
}

float gameplay_easing(uint8_t func_index, float value_0_to_1) {
    float v = value_0_to_1;
    switch (func_index) {
        case 0: return v;
        case 1: return cos(v * GLM_PI / 2);
        case 2: return v * v;
        default: printf("gameplay, easing --> no such easing function"); return 0;
    }
}

Gameplay_Player gameplay_player(GameplayPlayerStats stats) {
    return (Gameplay_Player) {
        .ent2 = entity2(),
        .health = stats.health
    };
}

void gameplay_player_lookAt(Entity* self, Entity target) {
    vec2 diff;
    glm_vec2_sub(target.pos, self->pos, diff);
    glm_vec2_normalize(diff);
    self->angle = atan2f(self->pos[1], self->pos[0]);
}

Gameplay_Player gameplay_player_physStep(Gameplay_Player self, GameplayPlayerStats stats) {
    float* vel = self.ent2.vel;
    float* pos = self.ent2.ent.pos;
    if (self.onGround) for (uint8_t n = 0; n < 3; n++) {
        pos[n] = (pos[n] + vel[n]) * (stats.friction / 100.0);
    }
    vel[2] -= 0.05;
    pos[2] -= vel[2];
    if (pos[2] < 0) pos[2] = 0;
    return self;
}

Gameplay_Bull gameplay_bull() {
    return (Gameplay_Bull) {.ent3 = entity3(), .beenOnGround = false, .seconds = 0};
}
