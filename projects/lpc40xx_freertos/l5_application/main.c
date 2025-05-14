#include "FreeRTOS.h"
#include "clock.h"
#include "draw.h"
#include "gpio.h"
#include "graphics.h"
#include "led_driver.h"
#include "object.h"
#include "task.h"
#include "uart.h"
#include <stdbool.h> // Add this if not already present
#include <stdio.h>
#include <stdlib.h> // For abs()
#include <string.h>
#include "board_io.h"
#include "accelerometer_task.h"
#include "game_task.h"

// constants
#define MATRIX_WIDTH 64
#define MATRIX_HEIGHT 64
#define CHAR_WIDTH 6

// global variables
// GameState current_state = MENU_STATE;
static gpio_s led0, led1;
extern uint8_t game_screen_state;

static void blink_task(void *params);

int main(void) {
  uart__init(UART__0, clock__get_peripheral_clock_hz(), 115200);
  led_driver__init_gpio_pins_for_matrix();
  srand(time(0));
  
  // for blink task
  led0 = board_io__get_led0();
  led1 = board_io__get_led1();

  // Clear matrix initially
  led_driver__clear_matrix();
  led_driver__update_display();
  
  xTaskCreate(blink_task, "led0", configMINIMAL_STACK_SIZE, (void *)&led0, PRIORITY_LOW, NULL);
  xTaskCreate(blink_task, "led1", configMINIMAL_STACK_SIZE, (void *)&led1, PRIORITY_LOW, NULL);
  xTaskCreate(accelerometer_task, "acc_task", (1024 / sizeof(void *)), NULL, PRIORITY_LOW, NULL);
  xTaskCreate(display_update_task, "DisplayUpdate", 2048, NULL, PRIORITY_LOW, NULL);
  xTaskCreate(uart_control_task, "UartControl", 2048, NULL, PRIORITY_LOW, NULL);

  vTaskStartScheduler();
  return 0;
}

void blink_task(void *params) {
  const gpio_s led = *((gpio_s *)params);
  while (true) {
    gpio__toggle(led);
    vTaskDelay(500);
  }
}