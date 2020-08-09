#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>

#include "util.h"
#include "board.h"
#include "cpu.h"
#include "fan.h"
#include "signal.h"
#include "config.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define CLAMP(x, upper, lower) (MIN(upper, MAX(x, lower)))

config_t g_config;
ubnt_board_t g_board;

int main(void) {
  short fan_speed;
  unsigned char max_cpu_id;
  unsigned int temperature;

  signal(SIGTERM, sighandler);
  read_config(&g_config);
  board_init(&g_board, &g_config);

  printf("ubnt-fan-speed: Detected board id: 0x%04x\n", g_board.id);
  if (g_board.id == 0) {
    fprintf(stderr, "ubnt-fan-speed: Unsupported board\n");
    return 1;
  }

  printf("ubnt-fan-speed: Starting...\n");

  max_cpu_id = 3;
  fan_speed = g_board.minimum_fan_speed;
  g_board.set_online_cpus(max_cpu_id);
  g_board.set_fan_mode(FAN_MODE_MANUAL);
  g_board.set_fan_speed(fan_speed);

  while (1) {
    /* Make sure fan is in manual mode ! */
    g_board.set_fan_mode(FAN_MODE_MANUAL);

    temperature = g_board.get_temperature();

    /* If temperature reach 94°C set fan speed to 100% */
    if (temperature >= g_board.temperature_threshold_94 && fan_speed < 255) {
      fan_speed = 255;
      g_board.set_fan_speed(fan_speed);
    }
    /* If temperature reach 84°C progressively increase fan speed */
    else if (temperature >= g_board.temperature_threshold_84 && fan_speed < 255) {
      fan_speed = CLAMP(fan_speed + 5, g_board.minimum_fan_speed, 255);
      g_board.set_fan_speed(fan_speed);
    }
    /* If temperature less or equal 80°C progressibely reduce fan speed */
    else if (temperature <= g_board.temperature_threshold_80 && fan_speed > g_board.minimum_fan_speed) {
      fan_speed = CLAMP(fan_speed - 5, g_board.minimum_fan_speed, 255);
      g_board.set_fan_speed(fan_speed);
    }

    /* If temperature is lower or equal than 85°C enable back all cpu cores */
    if (temperature <= g_board.temperature_threshold_85 && max_cpu_id < 3) {
      max_cpu_id = 3;
      g_board.set_online_cpus(max_cpu_id);
    }
    /* If temperature reach 98°C disable cpu core 3 */
    else if (temperature >= g_board.temperature_threshold_98 && max_cpu_id > 2) {
      max_cpu_id = 2;
      g_board.set_online_cpus(max_cpu_id);
    }
    /* If temperature reach 99°C disable cpu cores 2,3 */
    else if (temperature >= g_board.temperature_threshold_99 && max_cpu_id > 1) {
      max_cpu_id = 1;
      g_board.set_online_cpus(max_cpu_id);
    }
    /* If temperature reach 100°C disable cpu cores 1,2,3 */ 
    else if (temperature >= g_board.temperature_threshold_100 && max_cpu_id > 0) {
      max_cpu_id = 0;
      g_board.set_online_cpus(max_cpu_id);
    }

    sleep(1);
  }

  return 1;
}

