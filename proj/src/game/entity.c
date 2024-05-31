#include "entity.h"


bool check_collision(Ball *ball, CollisionEntity *entity, CollisionType type) {
    unsigned int ball_critical;
    bool x_alignment, y_alignment;

    switch (type) {
        case PLAYER: {
            const Player *player = &entity->player;
            ball_critical = ball->x_pos + ball->radius * player->number == 1 ? 1 : -1;
            y_alignment = ball->y_pos <= player->y_pos && ball->y_pos >= player->y_pos + player->height;
            x_alignment = player->number == 1 ?
                ball_critical <= player->x_pos + player->width :
                ball_critical >= player->x_pos;
            break;
        }
        case WALL:
            const Wall *wall = &entity->wall;
            ball_critical = ball->y_pos + ball->radius * wall->top ? 1 : -1;
            y_alignment = wall->top ?
                ball_critical <= wall->y_pos + wall->height :
                ball_critical >= wall->y_pos - wall->height;

            x_alignment = true;
            break;
    }

    return x_alignment && y_alignment;
}