#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "draw.h"
#include "graphics.h"
#include "object.h"

#define CAR_WIDTH (4)
#define CAR_HEIGHT (5)
#define CAR_HEIGHT_WITH_PADDING (6)
#define CAR_WIDTH_WITH_PADDING (6)
#define NUM_OF_OBSTACLES (10)
#define LEVEL_1_SCORE (30)
#define LEVEL_2_SCORE (100)
#define LEVEL_3_SCORE (200)
#define LED_MATRIX_WIDTH (64)

extern SemaphoreHandle_t countdown;
extern SemaphoreHandle_t play;
extern SemaphoreHandle_t level;

const uint8_t car[] = {0x30, 0x78, 0x30, 0x78, 0x30, 0};

const uint8_t number[][5] = {{0xE0, 0xA0, 0xA0, 0xA0, 0xE0}, {0xE0, 0x40, 0x40, 0xC0, 0x40},
                             {0xE0, 0x80, 0xE0, 0x20, 0xE0}, {0xE0, 0x20, 0x60, 0x20, 0xE0},
                             {0x40, 0xE0, 0xC0, 0x80, 0x80}, {0xE0, 0x20, 0xE0, 0x80, 0xE0},
                             {0xE0, 0Xa0, 0xE0, 0x80, 0XE0}, {0x20, 0x20, 0x20, 0x20, 0xE0},
                             {0xE0, 0xA0, 0xE0, 0xA0, 0xE0}, {0xE0, 0x20, 0xE0, 0xA0, 0xE0}};

static void move_obstacles(bitmap_object *obstacle);

static uint16_t truck_bitmap[4][12] = {
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0xF000, 0xF000, 0xF000},
    {0xFFFF, 0x0780, 0x0780, 0x0780, 0x0780, 0x0780, 0x0780, 0xFFFF, 0xFFFF, 0xF000, 0, 0xF000},
    {0xFFFF, 0x0780, 0, 0, 0, 0, 0x0780, 0xFFFF, 0xFFFF, 0xF000, 0xF780, 0xF000},
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0xF000, 0xF000, 0xF000}};

static uint16_t score = 0;
uint8_t game_screen_state = START_SCREEN;
GameState current_state = MENU_STATE;

int collided_truck_x;
int collided_truck_y;
int collision_player1_x;
int collision_player1_y;

uint8_t current_level = 1;
static uint8_t num_of_on_screen_obstacles = 0;
static bitmap_object player_car1;
static bitmap_object player_car2;
static bitmap_object player_car;
bitmap_object start_screen_car = {.color = CYAN,
                                  .x = (LED_MATRIX_WIDTH / 2) - (CAR_WIDTH_WITH_PADDING / 2),
                                  .y = 5,
                                  .image = car,
                                  .height = CAR_HEIGHT_WITH_PADDING,
                                  .width = CAR_WIDTH_WITH_PADDING,
                                  .direction = RIGHT,
                                  .isAlive = true};

bitmap_object car_obstacle[NUM_OF_OBSTACLES];
static level_t levels[] = {{
                               .level_obstacle_mod = 1,
                               .num_of_obstacles = 3,
                               .score_per_car = 3,
                           },
                           {
                               .level_obstacle_mod = 3,
                               .num_of_obstacles = 3,
                               .score_per_car = 5,
                           },
                           {
                               .level_obstacle_mod = 4,
                               .num_of_obstacles = 4,
                               .score_per_car = 10,
                           }};

bitmap_object obstacle_types[] = {
    {
     .movement_type = DOWN,
     .height = 12,
     .width = 4,
     .speed = 30,
     .isAlive = true}
};

static void object__init_player_car1(void) {
  player_car1.x = 46;
  player_car1.y = 56;
  player_car1.height = 3;
  player_car1.width = 3;
}

int get_player_car_x1() { return player_car1.x; }

int get_player_car_x2() { return player_car2.x; }

void set_player_car_x1(int new_x) { player_car1.x = new_x; }

void set_player_car_x2(int new_x) { player_car2.x = new_x; }

static void object__init_player_car2(void) {
  player_car2.x = 15;
  player_car2.y = 56;
  player_car2.height = 3;
  player_car2.width = 3;
}

void game_init() {
  for (uint8_t i = 0; i < 4; i++) {
    car_obstacle[i].isAlive = false;
  }
  num_of_on_screen_obstacles = 0;
  current_level = 1;
  score = 0;
  object__init_player_car1();
  object__init_player_car2();
}

void object__draw(bitmap_object object) {
  drawBitmap(object.x, object.y, object.image, object.width, object.height, object.color);
}

void draw_car_object1(bitmap_object object) {
  fillRect(object.x, object.y, 1, 3, 0x001F);     // Left vertical bar
  fillRect(object.x + 2, object.y, 1, 3, 0x001F); // Right vertical bar
  fillRect(object.x + 1, object.y + 1, 1, 1, 0xF780);
}

void draw_car_object2(bitmap_object object) {
  fillRect(object.x, object.y, 1, 3, 0x0780);     // Left vertical bar
  fillRect(object.x + 2, object.y, 1, 3, 0x0780); // Right vertical bar
  fillRect(object.x + 1, object.y + 1, 1, 1, 0xF780);
}

void get_score(uint8_t *ten, uint8_t *unit) {
  uint16_t temp_score = score;
  *unit = temp_score % 10;
  temp_score /= 10;
  *ten = temp_score % 10;
  temp_score /= 10;
}

void draw_player_car1() { draw_car_object1(player_car1); }
void draw_player_car2() { draw_car_object2(player_car2); }

void draw_truck(bitmap_object obstacle) {
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 12; y++) {
      uint16_t pixel = obstacle.obstacle_matrix[x][y];
      int pixel_x = obstacle.x + x;
      int pixel_y = obstacle.y + y;
      if (pixel_y >= 0 && pixel_y < MATRIX_WIDTH && pixel_x >= 0 && pixel_x < MATRIX_HEIGHT) {
        color_code_s color;
        switch (pixel) {
        case 0xFFFF:
          color = WHITE;
          break;
        case 0x0780:
          color = RED;
          break;
        case 0xF000:
          color = GREEN;
          break;
        case 0xF780:
          color = YELLOW;
          break;
        default:
          color = BLACK;
          break;
        }
        led_driver__set_pixel(pixel_x, pixel_y, color);
      }
    }
  }
}

static void draw_obstacles() {
  for (uint8_t i = 0; i < 4; i++) {
    if (car_obstacle[i].isAlive) {
      draw_truck(car_obstacle[i]);
    }
  }
}

/*****************************************************************************
 *                 MOVE FUNCTIONS -> BEGIN
 *
 *****************************************************************************/

void move_truck(bitmap_object *truck) {
  // truck->counter++;
  // if (truck->counter < truck->speed) {
  //   return;
  // }

  truck->y++; // Increase y to move downwards

  if (truck->y > MATRIX_HEIGHT) {
    truck->isAlive = false;
    score++;
    // Add any scoring or game over logic here if needed
    // if (score > LEVEL_1_SCORE && score < LEVEL_2_SCORE && current_level != 2) {
    //   current_level = 2;
    // } else if (score > LEVEL_2_SCORE && current_level != 3) {
    //   current_level = 3;
    // }
    num_of_on_screen_obstacles--;
  }
}

void move() {
  for (uint8_t i = 0; i < 4; i++) {
    if (car_obstacle[i].isAlive) {
      move_truck(&car_obstacle[i]);
    }
  }
}

void move_car_left() {
  if (current_state != GAME_STATE) {
    return;
  }
  if (player_car1.x < 58) {
    player_car1.x += 1;
  }
}

void move_car_right() {
  if (current_state != GAME_STATE) {
    return;
  }
  if (player_car1.x > (34)) {
    player_car1.x -= 1;
  }
}

void move_start_screen_car() {

  if (start_screen_car.direction == RIGHT) {
    if (start_screen_car.x < (LED_MATRIX_WIDTH - BORDER_WIDTH - CAR_WIDTH_WITH_PADDING)) {
      start_screen_car.x = start_screen_car.x + 1;
    } else {
      start_screen_car.direction = LEFT;
    }
  } else {
    if (start_screen_car.x > BORDER_WIDTH) {
      start_screen_car.x = start_screen_car.x - 1;
    } else {
      start_screen_car.direction = RIGHT;
    }
  }
}

/*****************************************************************************
 *                 MOVE FUNCTIONS -> END
 *
 *****************************************************************************/

void draw_countdown_screen() {
  uint8_t i = 3;
  while (i > 0) {
    draw_player_car1();
    draw_player_car2();
    draw_roads_and_borders();
    // drawBitmap(, number[i], 3, 5, GREEN);
    draw_score();
    vTaskDelay(1 / portTICK_PERIOD_MS);
    i--;
  }
}

void draw() {
  draw_player_car1();
  draw_player_car2();
  draw_roads_and_borders();
  draw_obstacles();
  draw_score();
  // draw_level();
}

/*****************************************************************************
 *                 OBSTACLES -> BEGIN
 *
 *****************************************************************************/

static void move_obstacles(bitmap_object *obstacle) {
  // obstacle->counter++;
  // if (obstacle->counter < obstacle->speed) {
  //   return;
  // }

  switch (obstacle->movement_type) {
  case DOWN:
    obstacle->x = obstacle->x + 1;
    break;
  // case DOWN_AND_LEFT_RIGHT:
  //   obstacle->x = obstacle->x + 1;
  //   if (obstacle->direction == RIGHT) {
  //     if (obstacle->y < (LED_MATRIX_WIDTH - BORDER_WIDTH - CAR_WIDTH_WITH_PADDING)) {
  //       obstacle->y = obstacle->y + 1;
  //     } else {
  //       obstacle->direction = LEFT;
  //     }
  //   } else {
  //     if (obstacle->y > BORDER_WIDTH) {
  //       obstacle->y = obstacle->y - 1;
  //     } else {
  //       obstacle->direction = RIGHT;
  //     }
  //   }
  // }

  // if (obstacle->x == (-1) * CAR_HEIGHT_WITH_PADDING) {
  //   obstacle->isAlive = false;
  //   score += levels[current_level - 1].score_per_car;
  //   if (score > LEVEL_1_SCORE && score < LEVEL_2_SCORE && current_level != 2) {
  //     xSemaphoreGive(level);
  //     current_level = 2;
  //   } else if (score > LEVEL_2_SCORE && current_level != 3) {
  //     xSemaphoreGive(level);
  //     current_level = 3;
  //   }
    num_of_on_screen_obstacles--;
  }
}

static void generate_obstacle(bitmap_object *obstacle, uint8_t x) {
    obstacle->x = x;
    obstacle->y = 7; // Initial y-position

    // Copy truck bitmap into obstacle's matrix
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 12; j++) {
            obstacle->obstacle_matrix[i][j] = truck_bitmap[i][j];
        }
    }

    obstacle->isAlive = true;
    obstacle->height = 12;
    obstacle->width = 4;
    obstacle->speed = 5; // Default speed
}

void generate_random_obstacles() {
    static int frame_counter = 0;
    int next_generation_time = 20 + (rand() % 50); // Random between 20-70 frames

    frame_counter++;

    if (frame_counter >= next_generation_time) {
        if (num_of_on_screen_obstacles < 4) {
            for (uint8_t i = 0; i < 4; i++) {
                if (!car_obstacle[i].isAlive) {
                    uint8_t new_x;
                    bool valid_position = false;

                    // Attempt to generate a valid position
                    for (int attempts = 0; attempts < 10 && !valid_position; attempts++) {
                        new_x = 34 + (rand() % 24); // Random x between 34 and 57
                        valid_position = true;

                        // Check against existing obstacles
                        for (uint8_t j = 0; j < 4; j++) {
                            if (car_obstacle[j].isAlive) {
                                if (abs(new_x - car_obstacle[j].x) < 6) {
                                    valid_position = false; // Too close, retry
                                    break;
                                }
                            }
                        }
                    }

                    // If a valid position is found, generate the obstacle
                    if (valid_position) {
                        generate_obstacle(&car_obstacle[i], new_x);
                        num_of_on_screen_obstacles++;
                        break; // Generate one obstacle at a time
                    }
                }
            }
        }

        // Reset frame counter and pick a new random interval
        frame_counter = 0;
        next_generation_time = 20 + (rand() % 50); // Random between 20-70 frames
    }
}

/*****************************************************************************
 *                 OBSTACLES -> END
 *
 *****************************************************************************/

/*****************************************************************************
 *                 COLLISION DETECTION -> BEGIN
 *
 *****************************************************************************/

static bool check_collision(const bitmap_object obstacle) {
  bool check_y = false;
  if (((obstacle.x + 1) >= player_car1.x + 1) && ((obstacle.x + 1) <= (player_car1.x + 4))) {
    check_y = true;
    collided_truck_x = obstacle.x;
    collided_truck_y = obstacle.y; 
  } else if (((obstacle.x + 4) >= (player_car1.x + 1)) &&
             ((obstacle.x + 4) <= (player_car1.x + 4))) {
    check_y = true;
    collided_truck_x = obstacle.x;
    collided_truck_y = obstacle.y; 
  }

  if (!check_y) {
    return false;
  }

  if (((obstacle.y >= player_car1.y) && (obstacle.y <= (player_car1.y + 12 - 1))) ||
      (((obstacle.y + 12 - 1) >= player_car1.y) &&
       ((obstacle.y + 12 - 1) <= (player_car1.y + 12 - 1)))) {
    collided_truck_x = obstacle.x;
    collided_truck_y = obstacle.y;    
    return true;
  }
  return false;
}

void collision_detector() {
  for (uint8_t i = 0; i < 4; i++) {
    if (car_obstacle[i].isAlive) {
      if (check_collision(car_obstacle[i])) {
        collision_player1_x = player_car1.x;
        collision_player1_y = player_car1.y;
        current_state = GAME_OVER_STATE;
      }
    }
  }
}

/*****************************************************************************
 *                 COLLISION DETECTION ENDS
 *
 *****************************************************************************/