#include <cglm/cglm.h>

typedef struct {
    vec3 pos; // x, y, height (0, 0, 0 = center and on ground, negatives for underground)
    float angle; // 0 = upwards
} Entity;

// ---

typedef struct {
    Entity ent;
    vec3 vel;
    float scale; // 0, 1, 2 = point, normal, double, etc...
    uint8_t type; // 0, 1, 2, 3 = fixed size, laser, double ended laser, expandable
    bool solid; // false if I can move through things
} Entity2;

Entity2 entity2();

// ---

typedef struct {
    Entity2 ent2;
    uint16_t health; // 0, 100 = defeated, starting, also can exceed 100
    int8_t health_changePerSec; // 0, positive, negative = normal, regenerating, decaying
    uint8_t friction; // 0, 100 = superglue, normal
    uint8_t acceleration; // 0, 100 = no strength, normal
    int16_t armour_lower; // -1, 0, 100 = non-existant, none, normal
    int16_t armour_mid; // -1, 0, 100 = non-existant, none, normal
    int16_t armour_upper; // -1, 0, 100 = non-existant, none, normal
    uint16_t strength; // 0, 100 = slowed to a stop carrying anything, normal strength
    char* name; // for flavour, hello my name is ...
} GameplayPlayerStats;

GameplayPlayerStats gameplay_player_stats(uint8_t choice);

int gameplay_player_stats_count = 3;

// ---

typedef struct {
    Entity2 ent2;
    bool onGround; // on the ground now
} Entity3;

Entity3 entity3();

// ---

typedef struct {
    bool accelerating;
    float* values;
    float** weights;
    float** biases;
    // values 0 to 2:
    //   angle
    //   speed
    //   hidden variable
} GameplayDrive;

uint8_t gameplay_drive_valuesCount = 3;

GameplayDrive gameplay_drive_physStep(GameplayDrive d);

GameplayDrive gameplay_drive_physStep2(GameplayDrive d);

// easing functions
// continuous and increasing
// both input and output stay in the range 0 to 1
// (input, output) starts at (0, 0) and ends at (1, 1)
float Gameplay_easing(uint8_t func_index, float value_0_to_1);

// ---

typedef struct {
    Entity2 ent2;
    GameplayDrive drive;
    uint16_t health; // 0, 100 = defeated, starting, also can exceed 100
    bool onGround;
} Gameplay_Player;

Gameplay_Player gameplay_player(GameplayPlayerStats stats);

// Sets the self orientation to point to target (assumes radians)
void gameplay_player_lookAt(Entity* self, Entity target);

Gameplay_Player gameplay_player_physStep(Gameplay_Player self, GameplayPlayerStats stats);

// ---

typedef struct { // includes bullets, lasers, grenades, and the such
    uint16_t type;
    uint32_t lifespan; // lifespan once fired, 0 for indefinite
} Gameplay_Bull_stats;

typedef struct {
    Gameplay_Bull_stats* stats;
    Entity3 ent3; // null if being carried
    bool beenOnGround; // ever been on the ground
    uint32_t seconds; // lifespan so far, unused if lifespan is 0
} Gameplay_Bull;

Gameplay_Bull gameplay_bull();

// ---

typedef struct {
    uint16_t type; // what item I am
} Gameplay_item_stats;

// ---

typedef struct {
    Gameplay_item_stats* stats;
    Entity2 ent3; // null if being carried
    Gameplay_Player* player; // the player carrying me, null if on the ground
} Gameplay_item;
