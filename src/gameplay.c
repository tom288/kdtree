#include "gameplay.h"
#include "utility.h"
#include <cglm/cglm.h>
#include <math.h>
#include "shader.h"

// TODO: move me into main, pass in to gameplay_physics_tick from main each frame
Gameplay_Player player1;

void move_test_rectangle(float x, float y, Shader* rectangle_shader) { // for testing only
    shader_set_2fv(*rectangle_shader, "translate", 1, (vec2){x, y});;
}

void gameplay_init() {
    player1 = gameplay_player(gameplay_player_stats(0));
}

// todo: remove me
void gameplay_test() {
    Gameplay_Player t = player1;
    printf("%f\n", *player1.drive.perpendicular_movement);
}

// TODO: finish this
void gameplay_physics_tick(float interval_sec, vec2 input, Shader* rectangle_shader) {
    GameplayDrive* drive = &player1.drive;
    *drive = gameplay_drive_physStep(*drive, input, interval_sec);
    // gameplay_drive_physStep2(*player1.drive);
    uint8_t rotational_velocity_index = 0;
    uint8_t speed_index = 0;
    PosRot* pos_rot = &player1.ent2.pos_rot;
    pos_rot->angle += *drive->angular_velocity * interval_sec;
    pos_rot->pos[0] += drive->values[speed_index];
    move_test_rectangle(player1.ent2.pos_rot.pos[0], player1.ent2.pos_rot.pos[1], rectangle_shader);
}

Entity2 entity2() {
    return (Entity2) {
        .pos_rot = {{0.0, 0.0, 0.0}, 0.0},
        .vel = {1.0, 1.0, 1.0},
        .scale = 1.0,
        .type = 0,
        .solid = false
    };
}

int gameplay_player_stats_count = 3;

GameplayPlayerStats gameplay_player_stats(uint8_t choice) {
    GameplayPlayerStats def = (GameplayPlayerStats) {
        .ent2 = (Entity2) {.pos_rot = {{0, 0, 0}, 0}, .vel = {0, 0, 0}, .scale = 1, .type = 3, .solid = true},
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

uint8_t gameplay_drive_valuesCount = 3;

GameplayDrive gameplay_drive() {
    GameplayDrive d = (GameplayDrive){
        .values = {0, 0, 0, 0},
        .weights = {{0, 0, 0, 0}, {0, 0, 0, 0}}};
    d.speed = &(d.values[0]);
    d.angular_velocity = &(d.values[1]);
    d.perpendicular_movement = &(d.values[2]);
    return d;
}

// todo: finish me
GameplayDrive gameplay_drive_physStep(GameplayDrive d, vec2 input, float interval_sec) {
    uint8_t rotation_speed_index = 2;
    float input_steering = input[0];
    float input_accelerating = input[1];
    if (input_accelerating > 0) *d.speed += 0.1 * interval_sec;
    *d.speed *= powf(0.5, interval_sec * 10.0);
    *d.angular_velocity += input_steering * interval_sec;
    *d.angular_velocity *= powf(0.5, interval_sec * 10.0);
    return d;
}

// todo: what easing function ID
GameplayDrive gameplay_drive_physStep2(GameplayDrive d) {
    for (uint8_t n = 0; n < gameplay_drive_valuesCount; n++) {
        for (uint8_t m = 0; m < gameplay_drive_valuesCount; m++) {
            d.values[n] = gameplay_easing(0, d.values[m]) * d.weights[n][m];
            // d.values[n] = gameplay_easing(0, d.values[m]) * d.weights[n][m] + d.biases[n][m];
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
        default: error("no such easing function"); return 0;
    }
}

Gameplay_Player gameplay_player(GameplayPlayerStats stats) {
    return (Gameplay_Player) {
        .ent2 = entity2(),
        .health = stats.health,
        .drive = gameplay_drive(),
    };
}

void gameplay_player_lookAt(PosRot* self, PosRot target) {
    vec2 diff;
    glm_vec2_sub(target.pos, self->pos, diff);
    glm_vec2_normalize(diff);
    self->angle = atan2f(self->pos[1], self->pos[0]);
}

Gameplay_Player gameplay_player_physStep(Gameplay_Player self, GameplayPlayerStats stats) {
    float* vel = self.ent2.vel;
    float* pos = self.ent2.pos_rot.pos;
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
