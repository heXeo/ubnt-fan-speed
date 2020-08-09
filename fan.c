#include <stdio.h>
#include <errno.h>

#include "util.h"
#include "fan.h"

void stub_set_fan_mode(__attribute__((unused)) const fan_mode_t mode) { return; }
unsigned char stub_get_fan_speed(void) { return 255; }
void stub_set_fan_speed(__attribute__((unused)) const unsigned char speed) { return; }

static void _set_fan_mode(const char* const path, const fan_mode_t mode) {
  int ret = file_write_uint(path, mode);
  if (ret <= 0) {
    fprintf(stderr, "ubnt-fan-speed: [ERR] Failed to write %s = %u (errno = %d, ret = %d)\n", path, mode, errno, ret);
  }
}

void legacy_set_fan_mode(const fan_mode_t mode) {
  _set_fan_mode("/sys/class/hwmon/hwmon0/pwm1_enable", mode);
}

void set_fan_mode(const fan_mode_t mode) {
  _set_fan_mode("/sys/class/hwmon/hwmon0/device/pwm1_enable", mode);
  _set_fan_mode("/sys/class/hwmon/hwmon0/device/pwm2_enable", mode);
}

static void _set_fan_speed(const char* const path, const unsigned char speed) {
  int ret = file_write_uint(path, speed);
  if (ret <= 0) {
    fprintf(stderr, "ubnt-fan-speed: [ERR] Failed to write %s = %u (errno = %d, ret = %d)\n", path, speed, errno, ret);
  }
}

void legacy_set_fan_speed(const unsigned char speed) {
  _set_fan_speed("/sys/class/hwmon/hwmon0/pwm1", speed);
}

void set_fan_speed(const unsigned char speed) {
  _set_fan_speed("/sys/class/hwmon/hwmon0/device/pwm1", speed);
  _set_fan_speed("/sys/class/hwmon/hwmon0/device/pwm2", speed);
}

static unsigned char _get_fan_speed(const char* const path) {
  unsigned int value = 255;
  int ret = file_read_uint(path, &value);
  if (ret <= 0) {
    fprintf(stderr, "ubnt-fan-speed: [ERR] Failed to read %s (errno = %d, ret = %d)\n", path, errno, ret);
  }
  return value;
}

unsigned char legacy_get_fan_speed(void) {
  return _get_fan_speed("/sys/class/hwmon/hwmon0/pwm1");
}

unsigned char get_fan_speed(void) {
  return _get_fan_speed("/sys/class/hwmon/hwmon0/device/pwm1");
}
