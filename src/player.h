#ifndef PLAYER_H
#define PLAYER_H

#include "utils/defs.h"

#define PLAYER_HEALTH 100

typedef struct player_s {
    v3f pos;
    v3f dir;
    v3f vel;
    int health;
} player_t;

player_t init_player(v3f pos, v3f dir);

#endif
