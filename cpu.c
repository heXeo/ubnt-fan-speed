#include <stdio.h>
#include <errno.h>

#include "util.h"

void stub_set_online_cpus(__attribute__((unused)) const unsigned char max_cpu_id) { return; }

void set_online_cpus(const unsigned char max_cpu_id) {
  unsigned char cpu_id = 1;
  char path[40];
  do {
    snprintf(path, 36, "/sys/devices/system/cpu/cpu%u/online", cpu_id);

    unsigned int state = cpu_id <= max_cpu_id;
    int ret = file_write_uint(path, state);
    if (ret <= 0) {
      fprintf(stderr, "ubnt-fan-speed: [ERR] Failed to write %s = %u (errno = %d, ret = %d)", path, state, errno, ret);
    }

    cpu_id++;
  } while (cpu_id < 4);
}
