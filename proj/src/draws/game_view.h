#ifndef _GAME_VIEW_H_
#define _GAME_VIEW_H_

/** @defgroup game_view game_view
 * @{
 *
 * Functions related to the view of the game
 */

/**
 * @brief In the current state of the game, draws the game
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_game();

/**@}*/

#endif /* _GAME_VIEW_H_ */


typedef struct {
  int16_t x;               /*!< horizontal coordinate of the button (top-left corner) */
  int16_t y;               /*!< vertical coordinate of the button (top-left corner) */
  int16_t w;               /*!< width of the button */
  int16_t h;               /*!< height of the button */
  bool selected;           /*!< true if the button is selected, false otherwise */
  Sprite *sprite;          /*!< sprite of the button (when unselected) */
  Sprite *sprite_selected; /*!< sprite of the button (when selected) */
} Button;