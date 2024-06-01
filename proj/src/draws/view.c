#include <lcom/lcf.h>

#include "view.h"

#include "game/model.h"

#include "game_view.h"
#include "menu_view.h"

#include "../controller/video.h"

#define COLOR_BACKGROUND 0x000000

extern State state;

int draw_frame() {
  if (video_draw_background(COLOR_BACKGROUND)) {
    printf("%s: video_draw_background(COLOR_BACKGROUND: 0x%x) error\n", __func__, COLOR_BACKGROUND);
    return 1;
  }

  if (state == MENU && draw_menu()) {
    printf("%s: draw_menu() error\n", __func__);
    return 1;
  }
  else if (state == GAME && draw_game()) {
    printf("%s: draw_game() error\n", __func__);
    return 1;
  }

  return 0;
}
