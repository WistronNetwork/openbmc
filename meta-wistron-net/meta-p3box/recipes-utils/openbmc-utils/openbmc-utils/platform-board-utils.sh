#!/bin/bash
#
# Copyright 2022-present Wistron. All Rights Reserved.
#

#shellcheck disable=SC1091
source /usr/local/bin/i2c-utils.sh

FANCPLD_SYSFS_DIR="$(i2c_device_sysfs_abspath 3-0016)/hwmon/hwmon*"
FAN_MAX=6
BMC_READY=GPIOD2
BMC_READY_VALUE=1

platform_boot_ready() {
    bmc_ready "enable"
}

platform_fw_fru_list() {
    echo "bmc mb fan psu xcvr"
}

platform_fw_component_list() {
    local fru=$1

    case "$fru" in
        "bmc") echo "bmc altbmc"
        ;;
        "mb") echo "cpld1 cpld2 vr"
        ;;
        "fan") echo "cpld3"
        ;;
        "psu") echo "psu1 psu2"
        ;;
        "xcvr") echo "port1 port2 port3 port4 port5 port6 port7 port8 port9 port10
             port11 port12 port13 port14 port15 port16 port17 port18 port19 port20
             port21 port22 port23 port24 port25 port26 port27 port28 port29 port30
             port31 port32"
        ;;
    esac
}

platform_do_mb_update() {
    local component=$1
    local image=$2

    extension=${image##*.}
    if [ "$extension" = "jed" ]; then
        if [ "$component" = "cpld1" ]; then
            bus=4
            addr=0x10
        elif [ "$component" = "cpld2" ]; then
            bus=4
            addr=0x14
        else
            return "$FW_STATUS_NOT_SUPPORTED"
        fi

        printf "\n"
        cpldupdate-i2c $bus $addr "$image"
        ret=$?
        if [ "$ret" -ne 0 ]; then
            ret=$FW_STATUS_FAILURE
        fi
    else
        printf "Invalid image format\n"
        ret=$FW_STATUS_FAILURE
    fi

    return "$ret"
}

platform_do_fan_update() {
    local component=$1
    local image=$2

    extension=${image##*.}
    if [ "$extension" = "jed" ]; then
        if [ "$component" = "cpld3" ]; then
            bus=4
            addr=0x1c
        else
            return "$FW_STATUS_NOT_SUPPORTED"
        fi

        printf "\n"
        cpldupdate-i2c $bus $addr "$image"
        ret=$?
        if [ "$ret" -ne 0 ]; then
            ret=$FW_STATUS_FAILURE
        fi
    else
        printf "Invalid image format\n"
        ret=$FW_STATUS_FAILURE
    fi
    return "$ret"
}
