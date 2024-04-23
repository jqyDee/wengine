#include "player.h"
#include "utils/defs.h"

player_t init_player(v3f pos, v3f dir) {
  return (player_t){
      {pos.x, pos.y, pos.z},
      {dir.x, dir.y, dir.z},
      {0, 0, 0},
      PLAYER_HEALTH,
  };
}
