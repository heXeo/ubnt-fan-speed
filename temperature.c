#include <stdio.h>
#include <errno.h>

#include "util.h"

int stub_get_temperature(void) { return 50000; }

static int _get_temperature(const char* const path) {
  int temperature = 200000;
  int ret = file_read_int(path, &temperature);
  if (ret <= 0) {
    fprintf(stderr, "ubnt-fan-speed: [ERR] Failed to read %s (errno = %d, ret = %d)", path, errno, ret);
  }
  return temperature;
}

int legacy_get_temperature(void) {
  return _get_temperature("/sys/class/thermal/thermal_zone0/temp");
}

int get_temperature(void) {
  int temperature1 = _get_temperature("/sys/class/thermal/thermal_zone0/temp");
  int temperature2 = _get_temperature("/sys/class/hwmon/hwmon0/temp2_input");

  if (temperature2 > 90000 && temperature2 > temperature1) {
    return temperature2;
  }

  return temperature1;
}
