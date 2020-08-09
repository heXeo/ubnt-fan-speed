#include <stdio.h>
#include <string.h>

#include "config.h"

void read_config(config_t* const config) {
  char buffer[256];
  char key[64];
  unsigned int value;

  config->log_level = 7;
  config->minimum_fan_speed = 0;
  config->force_minimum_fan_speed = 0;
  config->cpu_low_threshold = 85000;
  config->cpu_medium_threshold = 98000;
  config->cpu_high_threshold = 99000;
  config->cpu_critical_threshold = 100000;
  config->fan_low_threshold = 80000;
  config->fan_high_threshold = 84000;
  config->fan_critical_threshold = 94000;

  FILE* fp = fopen("/etc/ubnt/fan-speed.conf", "r");
  if (fp == NULL) {
    return;
  }

  while (1) {
    char* ptr = fgets(buffer, 256, fp);
    if (ptr == NULL) {
      break;
    }

    int read = sscanf(buffer, "%64[^=]=%u", key, &value);
    if (read == EOF || read != 2) {
      break;
    }

    if (strncmp(key, "log_level", 9) == 0) {
      config->log_level = value;
    } else if (strncmp(key, "minimum_fan_speed", 17) == 0) {
      config->minimum_fan_speed = value;
    } else if (strncmp(key, "force_minimum_fan_speed", 23) == 0) {
      config->force_minimum_fan_speed = value;
    } else if (strncmp(key, "cpu_low_threshold", 17) == 0) {
      config->cpu_low_threshold = value;
    } else if (strncmp(key, "cpu_medium_threshold", 20) == 0) {
      config->cpu_medium_threshold = value;
    } else if (strncmp(key, "cpu_high_threshold", 18) == 0) {
      config->cpu_high_threshold = value;
    } else if (strncmp(key, "cpu_critical_threshold", 22) == 0) {
      config->cpu_critical_threshold = value;
    } else if (strncmp(key, "fan_low_threshold", 17) == 0) {
      config->fan_low_threshold = value;
    } else if (strncmp(key, "fan_high_threshold", 18) == 0) {
      config->fan_high_threshold = value;
    } else if (strncmp(key, "fan_critical_threshold", 22) == 0) {
      config->fan_critical_threshold = value;
    }
  }
}
