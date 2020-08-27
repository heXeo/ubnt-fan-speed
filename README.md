# Ubiquiti ubnt-fan-speed

This repository contains a reversed engineered based source code of the /usr/sbin/ubnt-fan-speed executable found running as a service on some ubiquiti applicance (i.e: UDM Pro).

This work is based on the ubnt-fan-speed executable found on the UDM Pro firmware version 1.7.2.

## Motivation

The motivation to start this project is/was the lack of customization of the fan speed on my UDM Pro.
It was running too hot (at least from my point of view) and the fans were running at 0%.

It has been years since the last time i did a bit of reverse engineering and writting C code, so i though it was also a good opportunity to.

## What's different from the original ?

- [ ] More logs
- [ ] Fine grained log levels
- [x] Better error handling of fgets/sscanf (original code could lead to damaging behaviours)
- [x] Better error handling of fputs/fprintf (original code could lead to damaging behaviours)
- [x] Possibility to override `minimum_fan_speed` in /etc/ubnt/fan-speed.conf
- [x] Possibility to customize temperature thresholds in /etc/ubnt/fan-speed.conf
- [x] Replace `R_OK` by `F_OK` in `access` call
- [x] Replace `signal` by `sigaction`
- [x] Hot restart via `SIGHUP`

## Building from sources

```
apt-get install gcc-aarch64-linux-gnu
make
```

## Installation - UDM/UDMPro

- Install [on-boot-script](https://github.com/boostchicken/udm-utilities/tree/master/on-boot-script)
- Copy [on_boot.d/01-fan-speed.sh](on_boot.d/01-fan-speed.sh) to /mnt/data/on_boot.d/01-fan-speed.sh
- Make the boot script executable `chmod +x /mnt/data/on_boot.d/01-fan-speed.sh`
- Copy ubnt-fan-speed executable in /mnt/data/ubnt-fan-speed
- Create a configuration file at /mnt/data/ubnt-fan-speed.conf
- Reboot

## Installation - Other (Untested)

- Replace /usr/sbin/ubnt-fan-speed
- Create a configuration file at /etc/ubnt/fan-speed.conf
- Reboot

## Thoughts

I think the name of the executable doesn't reflect properly what it does.
It controls not only fans but also CPU cores enabling/disabling in order to control the temperature of the applicance.
Wouldn't a better name be `ubnt-thermal-control` ?

## Configuration file

The default configuration file location is `/etc/ubnt/fan-speed.conf`.

```
#
# log_level: Set the log level
# off=0, fatal=1, error=2, warn=3, info=4, debug=5, trace=6, all=7
#
log_level=4

#
# minimum_fan_speed: The minimum fan speed [0, 255]
# The effective minimum fan speed is clamped to the original minimum fan speed
# set by the service.
# For example the UDM Pro set the minimum fan speed to be around 50% (127) when
# a drive present.
#
minimum_fan_speed=255

#
# force_minimum_fan_speed: Force the minimum fan speed set by minimum_fan_speed
# overring the minimum fan speed set by the service.
# Set to 1 to enable.
# Warning: this could lead to hardware damage.
#
force_minimum_fan_speed=0

# The next section contains temperature threshold for CPU cores management.
# Based on those thresholds the service will enable/disable CPU cores in order
# to keep the temperature of the applicance below a critical level.
# As the temperature increase CPU cores will be disabled and vice versa.

#
# cpu_low_threshold: The lower temperature threshold in
# ten thousandth of degree celsius below which all CPU cores will be enabled.
#
cpu_low_threshold=85000

#
# cpu_medium_threshold: The first threshold in ten thousandth
# of degree celsius above which only 3 CPU cores will be left enabled.
#
cpu_medium_threshold=98000

#
# cpu_high_threshold: The second threshold in ten thousandth of
# degree celsius above which only 2 CPU cores will be left enabled.
#
cpu_high_threshold=99000

#
# cpu_critical_threshold: The third threshold in ten thousandth of
# degree celsius above which only 1 CPU cores will be left enabled.
#
cpu_critical_threshold=100000

# The next section contains temperature thresholds for the fans management.
# Based on those thresholds the service will set the fan speed in order
# to manage the temperature of the appliance.
# As the temperature increase the fans will spin faster and vice versa.

#
# fan_low_threshold: Set the threshold in ten thousandth of degree
# celsius at which the fan speed can start to be reduced.
# Below this threshold the fan speed will be reduced every second by 5.
#
fan_low_threshold=80000

#
# fan_high_threshold: Set the threshold in ten thousandth of
# degree celsius at which the fan speed starts to be increased.
# Above this threshold the fan speed will be increased every second by 5.
#
fan_high_threshold=84000

#
# fan_critical_threshold: Set the threshold in ten thousandth of degree
# celsius at which the fan speed will be set to 100% (255).
# Above this threshold the temperature is considered as critical and
# requires immediate cooling.
#
fan_critical_threshold=94000

```

## TODO

- Add ability to control chassis/hdd/cpu fans independently
- Check if drive is HDD or SSD
- Allow to tweak increase/decrease rate
- Add sensor sys file paths in config
- Add fan sys file paths in config

## License

Copyright (c) 2020 Leandre Gohy

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
