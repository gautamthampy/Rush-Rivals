#pragma once
#include <stdbool.h>
#include <stdint.h>

#define BORDER_HEIGHT (52)
#define BORDER_WIDTH (2)

enum move { DOWN = 0, DOWN_AND_LEFT_RIGHT = 1 };
enum direction { RIGHT = 0, LEFT = 1 };
enum screen_state {
  START_SCREEN = 0,
  GAME_SCREEN = 1,
  CAR_CRASH = 2,
  PAUSE_SCREEN = 3,
  GAME_OVER = 4,
  COUNTDOWN_SCREEN = 5
};
typedef enum { MENU_STATE, GAME_STATE, COUNTDOWN_STATE, GAME_OVER_STATE, PAUSE_STATE } GameState;

typedef struct {
  uint8_t level_obstacle_mod;
  uint8_t num_of_obstacles;
  uint8_t score_per_car;
} level_t;

typedef struct b {
  uint8_t x;
  int8_t y;
  const uint8_t *image; // ptr->image
  bool isAlive;
  uint8_t color;
  uint8_t movement_type;
  uint8_t height, width;
  uint8_t speed;
  uint8_t counter;
  uint8_t direction;
  uint16_t obstacle_matrix[4][12];
} bitmap_object;

// void object__init();
void move_car_left();
void move_car_right();
void draw();
void move();
void generate_random_obstacles();
void collision_detector();
void draw_countdown_screen();
void game_init();
void get_score(uint8_t *ten, uint8_t *unit);
void move_start_screen_car();
void object__draw(bitmap_object object);
void draw_player_car1();
void draw_player_car2();
void set_player_car_x1(int new_x);
void set_player_car_x2(int new_x);
int get_player_car_x1();
int get_player_car_x2();