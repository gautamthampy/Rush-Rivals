#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>

const uint8_t _width = 32;
const uint8_t _height = 64;

#define swap(a, b)                                                                                                     \
  {                                                                                                                    \
    int16_t t = a;                                                                                                     \
    a = b;                                                                                                             \
    b = t;                                                                                                             \
  }

void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) {

  int16_t i, j, byteWidth = (w + 7) / 8;

  switch (color) {
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
  case 0x001F:
    color = BLUE;
    break;
  case 0x7810:
    color = PURPLE;
    break;
  case 0x07FF:
    color = CYAN;
    break;
  default:
    color = BLACK;
    break;
  }

  for (j = 0; j < h; j++) {
    for (i = 0; i < w; i++) {
      if (pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
        led_driver__set_pixel(x + i, y + j, color);
      } else {
        led_driver__set_pixel(x + i, y + j, BLACK);
      }
    }
  }
}

void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  switch (color) {
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
  case 0x001F:
    color = BLUE;
    break;
  case 0x7810:
    color = PURPLE;
    break;
  case 0x07FF:
    color = CYAN;
    break;
  default:
    color = BLACK;
    break;
  }

  for (; x0 <= x1; x0++) {
    if (steep) {
      led_driver__set_pixel(y0, x0, color);
    } else {
      led_driver__set_pixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) { drawLine(x, y, x, y + h - 1, color); }

void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) { drawLine(x, y, x + w - 1, y, color); }

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  for (int16_t i = x; i < x + w; i++) {
    drawFastVLine(i, y, h, color);
  }
}

void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y + h - 1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x + w - 1, y, h, color);
}