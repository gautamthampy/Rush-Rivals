#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#include "draw.h"
#include "game_task.h"
#include "graphics.h"
#include "led_driver.h"
#include "object.h"

extern GameState current_state;

static void draw_game_screen() {
  generate_random_obstacles();
  move();
  draw();
  collision_detector();
}

// void game_task(void *params) {
//   game_screen_state = START_SCREEN;
//   while (true) {
//     switch (game_screen_state) {
//     case START_SCREEN:
//       draw_start_screen();
//       game_init();
//       break;
    // case COUNTDOWN_SCREEN:
    //   draw_countdown_screen();
    //   if (change_state) {
    //     change_state = false;
    //   }
    //   game_screen_state = GAME_SCREEN;
    //   break;
    // case GAME_SCREEN:
    //   xSemaphoreGive(car_moving);
    //   draw_game_screen();
    //   if (change_state) {
    //     game_screen_state = PAUSE_SCREEN;
    //     change_state = false;
    //   }
    //   break;
    // case PAUSE_SCREEN:
    //   xSemaphoreGive(no_sound);
    //   if (change_state) {
    //     game_screen_state = GAME_SCREEN;
    //     change_state = false;
    //   }
    //   break;
    // case CAR_CRASH:
    //   xSemaphoreGive(crash);
    //   draw_crash_screen();
    //   vTaskDelay(5000);
    //   led_matrix__turnOffAllPixels();
    //   game_screen_state = GAME_OVER;
    //   break;
    // case GAME_OVER:
    //   draw_gameover_screen();
    //   xSemaphoreGive(no_sound);
    //   if (change_state) {
    //     game_screen_state = START_SCREEN;
    //     change_state = false;
    //     led_matrix__turnOffAllPixels();
    //   }
    //   break;
//     }

//     vTaskDelay(50);
//   }
// }

void display_update_task(void *params) {
  while (true) {
    led_driver__clear_matrix();

    if (current_state == MENU_STATE) {
      draw_start_screen();
      game_init();
    } else if(current_state == COUNTDOWN_STATE) {
      draw_countdown_screen();
      current_state = GAME_STATE;
    }
    else if (current_state == GAME_STATE) {
      draw_game_screen();
    }
    else if (current_state == GAME_OVER_STATE) {
      draw_crash_screen();
    }
    led_driver__update_display();
    vTaskDelay(15 / portTICK_PERIOD_MS);
  }
}

void uart_control_task(void *params) {
  char command;
  while (true) {
    if (uart__get(UART__0, &command, 10)) {
      if (current_state == MENU_STATE) {
        current_state = COUNTDOWN_STATE;
      }
    } else if (current_state == GAME_OVER_STATE) {
      switch (command) {
        // case 'Y':
        // case 'y':
        //   current_state = COUNTDOWN_STATE;
        //   break;

        case 'N':
        case 'n':
          current_state = MENU_STATE;
          break;  
      }
    }
    // } else if (current_state == GAME_STATE) {
    //   switch (command) {

    //   case 'a':
    //   case 'A':
    //     if (get_player_car_x1() == 46) {
    //       set_player_car_x1(56);
    //     } else if (get_player_car_x1() == 36) {
    //       set_player_car_x1(46);
    //     }
    //     break;

    //   case 'd':
    //   case 'D':
    //     if (get_player_car_x1() == 56) {
    //       set_player_car_x1(46);
    //     } else if (get_player_car_x1() == 46) {
    //       set_player_car_x1(36);
    //     }
    //     break;

    //   case 'j':
    //   case 'J':
    //     if (get_player_car_x2() == 5) {
    //       set_player_car_x2(15);
    //     } else if (get_player_car_x2() == 15) {
    //       set_player_car_x2(25);
    //     }
    //     break;

    //   case 'l':
    //   case 'L':
    //     if (get_player_car_x2() == 25) {
    //       set_player_car_x2(15);
    //     } else if (get_player_car_x2() == 15) {
    //       set_player_car_x2(5);
    //     }
    //     break;
    //   }
    // }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}