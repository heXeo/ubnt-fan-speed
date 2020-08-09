#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "board.h"

void sighandler(const int signum) {
  if (signum == SIGTERM) {
    printf("ubnt-fan-speed: Shutting down...\n");
    g_board.set_fan_mode(FAN_MODE_AUTO);
    abort();
  }
}
