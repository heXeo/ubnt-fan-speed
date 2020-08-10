#!/bin/sh

if pidof ubnt-fan-speed &>/dev/null; then
  INIT_SCRIPT_PID=`pidof S04ubnt-fan-speed`
  UBNT_FAN_SPEED_PID=`pgrep -P ${INIT_SCRIPT_PID}`
  kill ${INIT_SCRIPT_PID} ${UBNT_FAN_SPEED_PID}
  sleep 1
fi

if test -f "/usr/sbin/ubnt-fan-speed"; then
  rm /usr/sbin/ubnt-fan-speed
  ln -s /mnt/data/ubnt-fan-speed /usr/sbin/ubnt-fan-speed 
fi

if ! test -f "/etc/ubnt/fan-speed.conf"; then
  ln -s  /mnt/data/ubnt-fan-speed.conf /etc/ubnt/fan-speed.conf
fi

/etc/init.d/S04ubnt-fan-speed start
