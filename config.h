#ifndef _CONFIG_H_
#define _CONFIG_H_

  struct config {
    unsigned char log_level;
    unsigned char minimum_fan_speed;
    unsigned char force_minimum_fan_speed;
    unsigned int cpu_low_threshold;
    unsigned int cpu_medium_threshold;
    unsigned int cpu_high_threshold;
    unsigned int cpu_critical_threshold;
    unsigned int fan_low_threshold;
    unsigned int fan_high_threshold;
    unsigned int fan_critical_threshold;
  };
  typedef struct config config_t;

  void read_config(config_t* const config);

#endif
