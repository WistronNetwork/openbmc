#!/bin/bash
#
# Copyright 2022-present Wistron. All Rights Reserved.
#

bios_boot_info() {
    printf "Not supported"
}

bios_boot_from() {
    printf "Not supported"
}

get_fan_speed() {
    #
    # Please use following format to print information in platform board-utils.sh
    # printf "Fan %s RPM: %s, PWM: %s %%\n" fan_unit rpm pwm
    #
    printf "Not supported"
    return 2
}

set_fan_speed() {
    #
    # Please use following format to print information in platform board-utils.sh
    # printf "Set Fan %s speed to %s %%\n" fan_unit pwm
    #
    printf "Not supported"
    return 2
}
