#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "acceleration.h"
#include "accelerometer_task.h"
#include "object.h"

void accelerometer_task(void *params) {

  acceleration__axis_data_s acc_sensor_values;
  int x = 0;
  while (1) {
    acc_sensor_values = acceleration__get_data();
    x = acc_sensor_values.x;

    switch (x) {
    case -149 ... 0:
      // printf("Go straight, y value=%lu\n", y);
      break;

    case 150 ... 1000:
      move_car_right();
      // printf("Go slight right, y value=%lu\n", y);
      break;

    case -1000 ... (-150):
      move_car_left();
      // printf("Go extreme left, y value=%lu\n", y);
      break;

    case 1 ... 149:
      // printf("Go straight, y value=%lu\n", y);
      break;

    default:
      break;
    }
    vTaskDelay(50);
  }
}