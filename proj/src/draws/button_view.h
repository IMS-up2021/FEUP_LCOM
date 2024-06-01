#ifndef _BUTTON_VIEW_H_
#define _BUTTON_VIEW_H_

#include "../menu/button.h"

/** @defgroup button_view button_view
 * @{
 *
 * Functions related to the view of the button
 */

/**
 * @brief Button drawer
 *
 * @param button pointer to the button that we need do be draw
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_button(Button *button);

/**@}*/

#endif /* _BUTTON_VIEW_H_ */
