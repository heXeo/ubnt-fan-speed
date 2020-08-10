#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#include "board.h"
#include "config.h"

static void sigterm_handler(const int signum) {
  if (signum == SIGTERM) {
    printf("ubnt-fan-speed: Shutting down...\n");
    g_board.set_fan_mode(FAN_MODE_AUTO);
    abort();
  }
}

static void sigterm_init(void) {
  struct sigaction new_action;
  struct sigaction old_action;

  new_action.sa_handler = sigterm_handler;
  sigemptyset(&new_action.sa_mask);
  new_action.sa_flags = 0;

  sigaction(SIGTERM, NULL, &old_action);
  if (old_action.sa_handler != SIG_IGN) {
    sigaction(SIGTERM, &new_action, NULL);
  }
}

static void sighup_handler(const int signum) {
  char buffer[256] = {0};
  if (signum == SIGHUP) {
    int read = readlink("/proc/self/exe", buffer, 256);
    if (read == -1) {
      // check errno
      return;
    }

    char* newargv[] = { buffer, "--config", (char*)g_config.path, NULL };

    printf("Restarting...\n");
    execv(buffer, newargv);
    printf("Error %d %s!\n", errno, buffer);
  }
}

static void sighup_init(void) {
  struct sigaction new_action;
  struct sigaction old_action;

  new_action.sa_handler = sighup_handler;
  sigemptyset(&new_action.sa_mask);
  new_action.sa_flags = SA_NODEFER;

  sigaction(SIGHUP, NULL, &old_action);
  if (old_action.sa_handler != SIG_IGN) {
    sigaction(SIGHUP, &new_action, NULL);
  }
}

void signal_init(void) {
  sigterm_init();
  sighup_init();
}
