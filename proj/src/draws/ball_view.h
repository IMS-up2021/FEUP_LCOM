#ifndef _BALL_VIEW_H_
#define _BALL_VIEW_H_

#include "../game/ball.h"

/** @defgroup ball_view ball_view
 * @{
 *
 * Functions related to the view of the ball
 */

/**
 * @brief Ball drawer
 *
 * @param ball pointer to the ball that we need do be draw
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_ball(Ball *ball);

/**@}*/

#endif /* _BALL_VIEW_H_ */
