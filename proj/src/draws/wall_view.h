#ifndef _WALL_VIEW_H_
#define _WALL_VIEW_H_

#include "../game/entity.h"

/** @defgroup wall_view wall_view
 * @{
 *
 * Functions related to the view of the wall
 */

/**
 * @brief Wall drawer
 *
 * @param wall  to the wall that we need do be draw
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_wall(Wall *wall);

/**@}*/

#endif /* _WALL_VIEW_H_ */
