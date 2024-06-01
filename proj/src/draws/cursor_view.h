#ifndef _CURSOR_VIEW_H_
#define _CURSOR_VIEW_H_

#include "../game/cursor.h"

/** @defgroup cursor_view cursor_view
 * @{
 *
 * Functions related to the view of the cursor
 */

/**
 * @brief Cursor drawer
 *
 * @param cursor pointer to the cursor that we need do be draw
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_cursor(Cursor *cursor);

/**@}*/

#endif /* _CURSOR_VIEW_H_ */
