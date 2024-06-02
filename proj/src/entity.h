#ifndef PROJ_ENTITY_H
#define PROJ_ENTITY_H

#include <stdbool.h>

typedef struct {
    const unsigned int x_pos;
    unsigned int y_pos;
    unsigned int width;
    unsigned int height;
    const unsigned char number;
} Player;

typedef struct {
    const unsigned int radius;
    unsigned int x_pos;
    unsigned int y_pos;
    unsigned int velocity;
    float angle;
} Ball;

typedef struct {
    const unsigned int x_pos;
    const unsigned int y_pos;
    const unsigned int width;
    const unsigned int height;
    const bool top;
} Wall;

typedef enum {
    PLAYER,
    WALL,
} CollisionType;

typedef union {
    Player player;
    Wall wall;
} CollisionEntity;

bool collision_handler(Ball *ball, CollisionEntity *entity, CollisionType type);

#endif
