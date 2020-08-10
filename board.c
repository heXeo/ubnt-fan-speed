#include <stdio.h>
#include <unistd.h>

#include "board.h"
#include "cpu.h"
#include "fan.h"
#include "temperature.h"
#include "util.h"
#include "config.h"

static unsigned int get_board_id(void) {
  char buffer[256];
  unsigned int board_id = 0;

  FILE* hal_board = fopen("/proc/ubnthal/board", "r");
  if (hal_board == NULL) {
    return board_id;
  }

  do {
    char* ptr = fgets(buffer, 0x100, hal_board);
    if (ptr == NULL) {
      break;
    }

    int count = sscanf(buffer, "boardid=%04x", &board_id);
    if (count == 1) {
      break;
    }
  } while (board_id == 0);

  return board_id;
}

ubnt_board_t* board_init(ubnt_board_t* const board, const config_t* const config) {
  board->id = get_board_id();
  board->minimum_fan_speed = 0x6d;
  board->cpu_low_threshold = config->cpu_low_threshold;
  board->cpu_medium_threshold = config->cpu_medium_threshold;
  board->cpu_high_threshold = config->cpu_high_threshold;
  board->cpu_critical_threshold = config->cpu_critical_threshold;
  board->fan_low_threshold = config->fan_low_threshold;
  board->fan_high_threshold = config->fan_high_threshold;
  board->fan_critical_threshold = config->fan_critical_threshold;
  board->set_fan_mode = NULL;
  board->set_fan_speed = NULL;
  board->get_fan_speed = NULL;
  board->get_temperature = NULL;
  board->set_online_cpus = NULL;

  switch (board->id) {
    case BOARD_UKNW_1:
      board->set_online_cpus = set_online_cpus;
      board->get_temperature = get_temperature;
      board->get_fan_speed = legacy_get_fan_speed;
      board->set_fan_speed = legacy_set_fan_speed;
      board->set_fan_mode = legacy_set_fan_mode;
      break;
    case BOARD_UKNW_2:
      /* NOTE: Is this a test board or a new board not released yet ? */
      board->set_online_cpus = stub_set_online_cpus;
      board->get_temperature = stub_get_temperature;
      board->get_fan_speed = stub_get_fan_speed;
      board->set_fan_speed = stub_set_fan_speed;
      board->set_fan_mode = stub_set_fan_mode;
      break;
    case BOARD_UDM_PRO: /* UDM-Pro */
    case BOARD_UKNW_3:
      /* If a drive is present in the drive bay, minimum fan speed is set to ~50% */
      if (file_exists("/dev/sdb") == 1) {
        board->minimum_fan_speed = 0x7f;
      } else {
        board->minimum_fan_speed = 0;
      }

      board->set_online_cpus = set_online_cpus;
      board->get_temperature = legacy_get_temperature;
      board->get_fan_speed = get_fan_speed;
      board->set_fan_speed = set_fan_speed;
      board->set_fan_mode = set_fan_mode;
      break;
    case BOARD_UKNW_4:
    case BOARD_UKNW_5:
    case BOARD_UKNW_6:
    case BOARD_UKNW_7:
    case BOARD_UKNW_8:
    default:
      board->id = 0;
      break;
  }

  if (config->minimum_fan_speed > board->minimum_fan_speed || config->force_minimum_fan_speed == 1) {
    board->minimum_fan_speed = config->minimum_fan_speed;
  }

  return board;
}
