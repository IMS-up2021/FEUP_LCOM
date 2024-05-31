#include "entity.h"


bool check_collision(Ball *ball, CollisionEntity entity, CollisionType type) {
    
    switch (type) {
        case PLAYER: {
            const Player player = entity.player;
            const unsigned int ball_critical = ball->x_pos + ball->radius * player.number == 1 ? 1 : -1;
            const bool y_alignment = ball->y_pos <= player.y_pos && ball->y_pos >= player.y_pos + player.height;
            const bool x_alignment = player.number == 1 ?
                ball_critical <= player.x_pos + player.width :
                ball_critical >= player.x_pos;

            return x_alignment && y_alignment;
        }
        case WALL:
            return;
    }

}