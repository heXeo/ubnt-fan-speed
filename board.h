#ifndef _BOARD_H_
#define _BOARD_H_

  #include "fan.h"
  #include "config.h"

  struct ubnt_board {
    unsigned short id;
    unsigned int temperature_threshold_98;
    unsigned int temperature_threshold_99;
    unsigned int temperature_threshold_100;
    unsigned int temperature_threshold_85;
    unsigned int temperature_threshold_84;
    unsigned int temperature_threshold_80;
    unsigned int temperature_threshold_94;
    unsigned char minimum_fan_speed;
    void (*set_fan_mode)(fan_mode_t mode);
    void (*set_fan_speed)(unsigned char speed);
    unsigned char (*get_fan_speed)(void);
    int (*get_temperature)(void);
    void (*set_online_cpus)(unsigned char max_cpu_id);
  };
  typedef struct ubnt_board ubnt_board_t;

  enum board_id {
    BOARD_UKNW_1 = 0xea11,
    BOARD_UKNW_2 = 0xea13,
    BOARD_UDM_PRO = 0xea15,
    BOARD_UKNW_3 = 0xea19,
    BOARD_UKNW_4 = 0xea14,
    BOARD_UKNW_5 = 0xea16,
    BOARD_UKNW_6 = 0xea17,
    BOARD_UKNW_7 = 0xea18,
    BOARD_UKNW_8 = 0xea12,

    /* Sourced from google */
    BOARD_UNIFI_AC_LITE = 0xe517,
    BOARD_UNIFI_AC_MESH = 0xe557,
    BOARD_POWER_STATION_5 = 0xb105,
    BOARD_POWER_STATION_2 = 0xb302,
    BOARD_ROCKET_M365 = 0xe1b3, /* POWER_BEAM_M3 ? */
    BOARD_POWER_BEAM_M5 = 0xe4e5,
    BOARD_NANO_STATION_M2 = 0xe012,
    BOARD_AIR_GATEWAY = 0xe4c7
  };
  typedef enum board_id board_id_t;

  ubnt_board_t g_board;
  ubnt_board_t* board_init(ubnt_board_t* const board, const config_t* const config);

#endif
