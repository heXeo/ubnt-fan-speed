#ifndef _FAN_H_
#define _FAN_H_

  enum fan_mode {
    FAN_MODE_MANUAL = 1,
    FAN_MODE_AUTO = 2
  };
  typedef enum fan_mode fan_mode_t;

  void stub_set_fan_mode(const fan_mode_t mode);
  unsigned char stub_get_fan_speed(void);
  void stub_set_fan_speed(const unsigned char speed);
  void set_fan_mode(const fan_mode_t mode);
  void legacy_set_fan_mode(const fan_mode_t mode);
  void set_fan_speed(const unsigned char speed);
  void legacy_set_fan_speed(const unsigned char speed);
  unsigned char get_fan_speed(void);
  unsigned char legacy_get_fan_speed(void);

#endif
