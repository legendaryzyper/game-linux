#ifndef WORLD_H
#define WORLD_H

#include "../gfx/gfx.h"
#include "../util/util.h"

#include "../entity/player.h"
#include "chunk.h"

typedef struct World {
	Player player;
	Chunk chunk;
} World;

void world_init(World* self);
void world_update(World* self);
void world_tick(World* self);
void world_render(World* self);
void world_destroy(World* self);

#endif
