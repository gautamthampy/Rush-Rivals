#include "draw.h"
#include "graphics.h"
#include "object.h"

#define LED_MATRIX_WIDTH (64)
#define LED_MATRIX_HEIGHT (64)
#define CHAR_WIDTH (6)

const uint8_t score_letter_S[] = {0xE0, 0x20, 0xE0, 0x80, 0xE0};
const uint8_t score_letter_C[] = {0xE0, 0x20, 0x20, 0x20, 0xE0};
const uint8_t score_letter_O[] = {0xE0, 0xA0, 0xA0, 0xA0, 0xE0};
const uint8_t score_letter_R[] = {0xE0, 0xA0, 0xE0, 0x60, 0xA0};
const uint8_t score_letter_E[] = {0xE0, 0x20, 0x60, 0x20, 0xE0};

uint16_t truck_bitmap_[4][12] = {
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0xF000, 0xF000, 0xF000},
    {0xFFFF, 0x0780, 0x0780, 0x0780, 0x0780, 0x0780, 0x0780, 0xFFFF, 0xFFFF, 0xF000, 0, 0xF000},
    {0xFFFF, 0x0780, 0, 0, 0, 0, 0x0780, 0xFFFF, 0xFFFF, 0xF000, 0xF780, 0xF000},
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0xF000, 0xF000, 0xF000}};

static const uint8_t number[][5] = {{0xE0, 0xA0, 0xA0, 0xA0, 0xE0}, {0x40, 0x60, 0x40, 0x40, 0xE0},
                                    {0xE0, 0x80, 0xE0, 0x20, 0xE0}, {0xE0, 0x80, 0xE0, 0x80, 0xE0},
                                    {0xA0, 0xA0, 0xE0, 0x80, 0x80}, {0xE0, 0x20, 0xE0, 0x80, 0xE0},
                                    {0xE0, 0x20, 0xE0, 0xA0, 0xE0}, {0xE0, 0x80, 0x80, 0x80, 0x80},
                                    {0xE0, 0xA0, 0xE0, 0xA0, 0xE0}, {0xE0, 0Xa0, 0xE0, 0x80, 0XE0}};    

extern int collided_truck_x;
extern int collided_truck_y;
extern int collision_player1_x;
extern int collision_player1_y;

void draw_score() {

  uint8_t ten, unit;

  // p1 blue score
  drawBitmap(60, 0, score_letter_S, 3, 5, 0x001F);
  drawBitmap(56, 0, score_letter_C, 3, 5, 0x001F);
  drawBitmap(52, 0, score_letter_O, 3, 5, 0x001F);
  drawBitmap(48, 0, score_letter_R, 3, 5, 0x001F);
  drawBitmap(44, 0, score_letter_E, 3, 5, 0x001F);

  // p2 red score
  drawBitmap(27, 0, score_letter_S, 3, 5, 0x0780);
  drawBitmap(23, 0, score_letter_C, 3, 5, 0x0780);
  drawBitmap(19, 0, score_letter_O, 3, 5, 0x0780);
  drawBitmap(15, 0, score_letter_R, 3, 5, 0x0780);
  drawBitmap(11, 0, score_letter_E, 3, 5, 0x0780);

  get_score(&ten, &unit);
  drawBitmap(39, 0, number[ten], 3, 5, 0XFFFF);
  drawBitmap(35, 0, number[unit], 3, 5, 0XFFFF);

  drawFastHLine(0, 6, 64, 0xFFFF);
}

void draw_roads_and_borders() {
  // green lines border
  drawFastVLine(0, 7, 57, 0xF000);
  drawFastVLine(63, 7, 57, 0xF000);
  drawFastVLine(1, 7, 57, 0xF000);
  drawFastVLine(62, 7, 57, 0xF000);

  drawFastVLine(31, 0, 64, 0xF780); // middle line left
  drawFastVLine(32, 0, 64, 0xF780); // middle line right

  // LANE1 - right
  fillRect(11, 10, 1, 4, 0xFFFF);
  fillRect(11, 20, 1, 4, 0xFFFF);
  fillRect(11, 30, 1, 4, 0xFFFF);
  fillRect(11, 40, 1, 4, 0xFFFF);
  fillRect(11, 50, 1, 4, 0xFFFF);
  fillRect(11, 60, 1, 4, 0xFFFF);

  // LANE2 - right
  fillRect(21, 10, 1, 4, 0xFFFF);
  fillRect(21, 20, 1, 4, 0xFFFF);
  fillRect(21, 30, 1, 4, 0xFFFF);
  fillRect(21, 40, 1, 4, 0xFFFF);
  fillRect(21, 50, 1, 4, 0xFFFF);
  fillRect(21, 60, 1, 4, 0xFFFF);

  // // LANE3 - left
  // fillRect(42, 10, 1, 4, 0xFFFF);
  // fillRect(42, 20, 1, 4, 0xFFFF);
  // fillRect(42, 30, 1, 4, 0xFFFF);
  // fillRect(42, 40, 1, 4, 0xFFFF);
  // fillRect(42, 50, 1, 4, 0xFFFF);
  // fillRect(42, 60, 1, 4, 0xFFFF);

  // // LANE3 - left
  // fillRect(52, 10, 1, 4, 0xFFFF);
  // fillRect(52, 20, 1, 4, 0xFFFF);
  // fillRect(52, 30, 1, 4, 0xFFFF);
  // fillRect(52, 40, 1, 4, 0xFFFF);
  // fillRect(52, 50, 1, 4, 0xFFFF);
  // fillRect(52, 60, 1, 4, 0xFFFF);
}

void draw_borders(void) {
  drawFastVLine(0, 7, 57, 0xF000);
  drawFastVLine(63, 7, 57, 0xF000);
  drawFastVLine(1, 7, 57, 0xF000);
  drawFastVLine(62, 7, 57, 0xF000);
}

const uint8_t crash_screen_B[] = {0xF0, 0X88, 0xF0, 0x88, 0xF0};
const uint8_t crash_screen_O[] = {0x70, 0x88, 0x88, 0x88, 0x70};
const uint8_t crash_screen_M[] = {0x88, 0x88, 0xA8, 0xD8, 0x88};
const uint8_t crash_screen_exclaimation[] = {0xC0, 0x00, 0xC0, 0xC0, 0xC0};

const uint8_t game_over_G[] = {0xF8, 0x88, 0xB8, 0x80, 0xF8};
const uint8_t game_over_A[] = {0x88, 0x88, 0xF8, 0x50, 0x20};
const uint8_t game_over_M[] = {0x88, 0x88, 0xA8, 0xD8, 0x88};
const uint8_t game_over_E[] = {0xF8, 0x80, 0xF8, 0x80, 0xF8};
const uint8_t game_over_O[] = {0x70, 0x88, 0x88, 0x88, 0x70};
const uint8_t game_over_V[] = {0x20, 0x50, 0x88, 0x88, 0x88};
const uint8_t game_over_R[] = {0x98, 0xA0, 0xF8, 0x88, 0xF8};

static const uint8_t smiley[] = {0x3c, 0x42, 0x99, 0xa5, 0x81, 0xa5, 0x81, 0x42, 0x3c};

const uint8_t level_letter_L[] = {0xE0, 0x80, 0x80, 0x80, 0x80};
const uint8_t level_letter_V[] = {0x40, 0xA0, 0xA0, 0xA0, 0xA0};

const uint8_t letter_R[] = {0x84, 0x88, 0x90, 0xA0, 0xC0, 0xFC, 0x84, 0x84, 0x84, 0xFC};
const uint8_t letter_O[] = {0x78, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x78};
const uint8_t letter_A[] = {0x84, 0x84, 0x84, 0x84, 0x84, 0xFC, 0x84, 0x84, 0x84, 0x78};
const uint8_t letter_D[] = {0xF0, 0x88, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x88, 0xF0};
const uint8_t letter_M[] = {0x84, 0x84, 0x84, 0x84, 0x84, 0xB4, 0xB4, 0xB4, 0xCC, 0x84};
const uint8_t letter_X[] = {0x84, 0x84, 0x84, 0x48, 0x30, 0x30, 0x48, 0x84, 0x84, 0x84};
const uint8_t letter_F[] = {0x80, 0x80, 0x80, 0x80, 0x80, 0xFC, 0x80, 0x80, 0x80, 0xFC};
const uint8_t letter_U[] = {0x78, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84};
const uint8_t letter_Y[] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x48, 0x84, 0x84, 0x84};

const uint8_t letter_smallP[] = {0x80, 0x80, 0xF8, 0x88, 0xF8};
const uint8_t letter_smallL[] = {0xF8, 0x80, 0x80, 0x80, 0x80};
const uint8_t letter_smallA[] = {0x88, 0x88, 0xF8, 0x88, 0X70};
const uint8_t letter_smallY[] = {0x20, 0x20, 0x20, 0x50, 0x88};

const uint8_t play_button[] = {0xC0, 0xE0, 0xF1, 0xE0, 0xC0};

void draw_collided_car_object1(int x, int y) {
  fillRect(x, y, 1, 3, 0x001F);     // Left vertical bar
  fillRect(x + 2, y, 1, 3, 0x001F); // Right vertical bar
  fillRect(x + 1, y + 1, 1, 1, 0xF780);
}

void draw_collided_truck(int x, int y) {
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 12; col++) {
      uint16_t pixel = truck_bitmap_[row][col];
      int pixel_x = x + row;
      int pixel_y = y + col;
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

void draw_crash_screen(void) {
  const char *title = "GAME OVER";

  // Calculate text positions once
  int title_x = 63-((MATRIX_WIDTH - (strlen(title) * CHAR_WIDTH)) / 2);

  display_text(title, title_x, 17, WHITE);
  draw_borders();
  draw_collided_car_object1(collision_player1_x, collision_player1_y);
  draw_collided_truck(collided_truck_x, collided_truck_y);
  draw_score();
}

void draw_gameover_screen() {
  uint8_t hundred, ten, unit;
  drawRect(0, 0, LED_MATRIX_WIDTH, LED_MATRIX_HEIGHT, WHITE);
  drawBitmap(4, 52, game_over_G, 5, 5, RED);
  drawBitmap(10, 52, game_over_A, 5, 5, RED);
  drawBitmap(16, 52, game_over_M, 5, 5, RED);
  drawBitmap(22, 52, game_over_E, 5, 5, RED);
  drawBitmap(4, 44, game_over_O, 5, 5, RED);
  drawBitmap(10, 44, game_over_V, 5, 5, RED);
  drawBitmap(16, 44, game_over_E, 5, 5, RED);
  drawBitmap(22, 44, game_over_R, 5, 5, RED);

  drawBitmap(6, 30, score_letter_S, 3, 5, BLUE);
  drawBitmap(10, 30, score_letter_C, 3, 5, BLUE);
  drawBitmap(14, 30, score_letter_O, 3, 5, BLUE);
  drawBitmap(18, 30, score_letter_R, 3, 5, BLUE);
  drawBitmap(22, 30, score_letter_E, 3, 5, BLUE);

  // get_score(&hundred, &ten, &unit);
  drawBitmap(10, 24, number[hundred], 3, 5, CYAN);
  drawBitmap(14, 24, number[ten], 3, 5, CYAN);
  drawBitmap(18, 24, number[unit], 3, 5, CYAN);

  drawBitmap(12, 8, smiley, 8, 9, YELLOW);
}

extern uint8_t current_level;

void draw_level() {
  drawBitmap(5, 53, level_letter_L, 3, 5, GREEN);
  drawBitmap(9, 53, score_letter_E, 3, 5, GREEN);
  drawBitmap(13, 53, level_letter_V, 3, 5, GREEN);
  drawBitmap(17, 53, score_letter_E, 3, 5, GREEN);
  drawBitmap(21, 53, level_letter_L, 3, 5, GREEN);

  drawBitmap(25, 53, number[current_level], 3, 5, GREEN);
}

extern bitmap_object start_screen_car;

/****************************************************************************
                                                                            *
                                  MENU SCREEN                               *
                                                                            *
****************************************************************************/

// font array for menu screen
const uint8_t font_5x7[][5] = {
    // Numbers 0-9
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // '0'
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // '1'
    {0x42, 0x61, 0x51, 0x49, 0x46}, // '2'
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // '3'
    {0x11, 0x14, 0x12, 0x7F, 0x10}, // '4'
    {0x27, 0x45, 0x45, 0x45, 0x39}, // '5'
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // '6'
    {0x01, 0x71, 0x09, 0x05, 0x03}, // '7'
    {0x36, 0x49, 0x49, 0x49, 0x36}, // '8'
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // '9'
                                    // Uppercase letters A-Z
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // 'A'
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // 'B'
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // 'C'
    {0x7F, 0x41, 0x41, 0x41, 0x3E}, // 'D'
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // 'E'
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // 'F'
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // 'G'
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // 'H'
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // 'I'
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // 'J'
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // 'K'
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // 'L'
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // 'M'
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // 'N'
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // 'O'
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // 'P'
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // 'Q'
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // 'R'
    {0x46, 0x49, 0x49, 0x49, 0x31}, // 'S'
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // 'T'
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // 'U'
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // 'V'
    {0x3F, 0x40, 0x38, 0x40, 0x3F}, // 'W'
    {0x63, 0x14, 0x08, 0x14, 0x63}, // 'X'
    {0x07, 0x08, 0x70, 0x08, 0x07}, // 'Y'
    {0x61, 0x51, 0x49, 0x45, 0x43}, // 'Z'
                                    // Space character
    {0x00, 0x00, 0x00, 0x00, 0x00}  // ' '
};

// get character index
int get_font_index(char c) {
  if (c >= '0' && c <= '9') {
    return c - '0'; // 0-9 maps to indices 0-9
  } else if (c >= 'A' && c <= 'Z') {
    return (c - 'A') + 10; // A-Z maps to indices 10-35
  } else if (c == ' ') {
    return 36; // Space is the last character
  }
  return -1; // Invalid character
}

// display character
void display_character(int start_row, int start_col, char c, color_code_s color) {
  int index = get_font_index(c);
  if (index == -1)
    return;

  for (int col = 0; col < 5; col++) {
    for (int row = 7; row >=0; row--) { // Back to 7 rows
      int adjusted_row = start_row + row;
      int adjusted_col = start_col - col;

      if (adjusted_row < MATRIX_HEIGHT && adjusted_col < MATRIX_WIDTH) {
        if (font_5x7[index][col] & (1 << row)) {
          led_driver__set_pixel(adjusted_col, adjusted_row, color);
        }
      }
    }
  }
}

// display word
void display_text(const char *text, int start_col, int start_row, color_code_s color) {
  int col = start_col;
  for (int i = 0; text[i] != '\0'; i++) {
    display_character(start_row, col, text[i], color);
    col -= CHAR_WIDTH; // Use consistent character width
  }
}

void draw_start_screen() {
  const char *title = "RIVAL RUSH";
  const char *prompt = "PRESS";
  const char *prompt2 = "ANY KEY";

  // Calculate text positions once
  int title_x = 63-((MATRIX_WIDTH - (strlen(title) * CHAR_WIDTH)) / 2);
  int prompt_x = 63-((MATRIX_WIDTH - (strlen(prompt) * CHAR_WIDTH)) / 2);
  int prompt2_x = 63-((MATRIX_WIDTH - (strlen(prompt2) * CHAR_WIDTH)) / 2);

  display_text(title, title_x, 5, WHITE);
  display_text(prompt, prompt_x, 44, WHITE);
  display_text(prompt2, prompt2_x, 44 + 9, WHITE);
  // object__draw(start_screen_car);
  // move_start_screen_car();
}