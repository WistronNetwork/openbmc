#!/bin/bash
#
# Copyright 2022-present Wistron. All Rights Reserved.
#

#shellcheck disable=SC1091
source /usr/local/bin/i2c-utils.sh

CPLD1_SYSFS_DIR="$(i2c_device_sysfs_abspath 3-0011)/hwmon/hwmon*"
CPLD2_SYSFS_DIR="$(i2c_device_sysfs_abspath 3-0012)/hwmon/hwmon*"
FANCPLD_SYSFS_DIR="$(i2c_device_sysfs_abspath 3-0016)/hwmon/hwmon*"
FAN_MAX=6
FAN_ROTOR=2
BMC_READY=GPIOD2
BMC_READY_VALUE=1

platform_boot_ready() {
    power-util psu on
    sleep 0.5
    bmc_ready enable

    for i in {1..32};
    do
        value=$(printf "%02d" "$i")
        if [ "$((i))" -le 16 ]; then
            led_path=$(realpath $CPLD1_SYSFS_DIR/qsfpdd_p"$value"_led_control)
        else
            led_path=$(realpath $CPLD2_SYSFS_DIR/qsfpdd_p"$value"_led_control)
        fi
        # CPLD control
        echo 7 > "$led_path"
        power-util xcvr on port"$i"
    done

    sfputil &> /dev/null
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

platform_do_mb_version() {
    local component=$1

    if [ "$component" = "cpld1" ]; then
        major_ver=$(head -n1 $CPLD1_SYSFS_DIR/cpld1_major_version 2> /dev/null)
        minor_ver=$(head -n1 $CPLD1_SYSFS_DIR/cpld1_minor_version 2> /dev/null)
        ret=$?
        if [ $((ret)) -eq 0 ]; then
            ver="$((major_ver)).$((minor_ver))"
        else
            ver="Get version error"
        fi
    elif [ "$component" = "cpld2" ]; then
        major_ver=$(head -n1 $CPLD2_SYSFS_DIR/cpld2_major_version 2> /dev/null)
        minor_ver=$(head -n1 $CPLD2_SYSFS_DIR/cpld2_minor_version 2> /dev/null)
        ret=$?
        if [ $((ret)) -eq 0 ]; then
            ver="$((major_ver)).$((minor_ver))"
        else
            ver="Get version error"
        fi
    elif [ "$component" = "vr" ]; then
        ver=$(i2cget -f -y 19 0x62 0x9b s)
        ret=$?
        if [ $((ret)) -ne 0 ]; then
            ver="Get version error"
        fi
    else
        ver="Not Supported"
        ret=$STATUS_NOT_SUPPORTED
    fi
    echo "$ver"

    return "$ret"
}

platform_do_mb_update() {
    local component=$1
    local image=$2

    extension=${image##*.}
    if [ "$extension" = "jed" ]; then
        device_name=$(cat < "$image" | grep -i "DEVICE NAME")
        design_name=$(cat < "$image" | grep -i "DESIGN NAME")
        if [ "$component" = "cpld1" ]; then
           if [[ "$device_name " = *LCMXO3LF-4300C-5CABGA324* ]] &&
              [[ "$design_name " = *SYS_CPLD* ]]; then
                bus=4
                addr=0x10
                gpiocli -c aspeed-gpio -n GPIOM3 -s FM_EN_PLD1_HITLESS_N export 2> /dev/null
                gpiocli -s FM_EN_PLD1_HITLESS_N set-init-value 0 2> /dev/null
                ret=$?
                if [ "$ret" -ne 0 ]; then
                    printf "Hitless setup fail\n"
                    return "$STATUS_FAILURE"
                fi
            else
                printf "Invalid image format\n"
                return "$STATUS_FAILURE"
            fi
        elif [ "$component" = "cpld2" ]; then
           if [[ "$device_name " = *LCMXO3LF-4300C-5CABGA324* ]] &&
              [[ "$design_name " = *LED_CPLD* ]]; then
                bus=4
                addr=0x14
            else
                printf "Invalid image format\n"
                return "$STATUS_FAILURE"
            fi
        else
            return "$STATUS_NOT_SUPPORTED"
        fi

        printf "\n"
        cpldupdate-i2c $bus $addr "$image"
        ret=$?
        if [ "$ret" -ne 0 ]; then
            ret=$STATUS_FAILURE
        fi

        if [ "$component" = "cpld1" ]; then
            gpiocli -s FM_EN_PLD1_HITLESS_N set-init-value 1 2> /dev/null
            gpiocli -c aspeed-gpio -s FM_EN_PLD1_HITLESS_N unexport 2> /dev/null
        fi
    else
        printf "Invalid image format\n"
        ret=$STATUS_FAILURE
    fi

    return "$ret"
}

platform_do_fan_version() {
    local component=$1

    if [ "$component" = "cpld3" ]; then
        major_ver=$(head -n1 $FANCPLD_SYSFS_DIR/fancpld_major_version 2> /dev/null)
        minor_ver=$(head -n1 $FANCPLD_SYSFS_DIR/fancpld_minor_version 2> /dev/null)
        ret=$?
        if [ $((ret)) -eq 0 ]; then
            ver="$((major_ver)).$((minor_ver))"
        else
            ver="Get version error"
        fi
    else
        ver="Not Supported"
        ret=$STATUS_NOT_SUPPORTED
    fi
    echo "$ver"

    return "$ret"
}

platform_do_fan_update() {
    local component=$1
    local image=$2

    extension=${image##*.}
    if [ "$extension" = "jed" ]; then
        device_name=$(cat < "$image" | grep -i "DEVICE NAME")
        design_name=$(cat < "$image" | grep -i "DESIGN NAME")
        if [ "$component" = "cpld3" ]; then
            if [[ "$device_name " = *LCMXO3LF-2100C-5CABGA324* ]] &&
               [[ "$design_name " = *FAN_CPLD* ]]; then
                bus=4
                addr=0x1c
            else
                printf "Invalid image format\n"
                return "$STATUS_FAILURE"
            fi
        else
            return "$STATUS_NOT_SUPPORTED"
        fi

        printf "\n"
        cpldupdate-i2c $bus $addr "$image"
        ret=$?
        if [ "$ret" -ne 0 ]; then
            ret=$STATUS_FAILURE
        fi
    else
        printf "Invalid image format\n"
        ret=$STATUS_FAILURE
    fi
    return "$ret"
}

platform_do_psu_version() {
    local component=$1

    if [ "$component" = "psu1" ]; then
        ver=$(i2cget -f -y 17 0x58 0xd9 s)
        ret=$?
        if [ $((ret)) -ne 0 ]; then
            ver="Get version error"
        fi
    elif [ "$component" = "psu2" ]; then
        ver=$(i2cget -f -y 17 0x59 0xd9 s)
        ret=$?
        if [ $((ret)) -ne 0 ]; then
            ver="Get version error"
        fi
    else
        ver="Not Supported"
        ret=$STATUS_NOT_SUPPORTED
    fi
    echo "$ver"

    return "$ret"
}

platform_do_xcvr_version() {
    local component=$1

    sfputil show fwversion --port "${component/port/}"
    ret=$?

    return "$ret"
}

platform_do_xcvr_update() {
    local component=$1
    local image=$2
    local mode=$3
    local commit=$4

    if [ "$commit" != "" ] && [ "$commit" != "0" ] && [ "$commit" != "1" ]; then
        return 1
    fi

    if [ "$mode" != "" ] && [ "$mode" != "0" ] && [ "$mode" != "1" ] &&
       [ "$mode" != "2" ] && [ "$mode" != "3" ]; then
        return 1
    fi

    if [ "$commit" = "" ]; then
        commit=1
    fi

    if [ "$mode" = "" ]; then
        mode=1
    fi

    sfputil firmware upgrade --port "${component/port/}" --filepath "$image" \
                             --commit "$commit" --mode "$mode"
    ret=$?

    return "$ret"
}

platform_power_fru_list() {
    echo "psu xcvr"
}

platform_power_component_list() {
    local fru=$1

    case "$fru" in
        "psu") echo "psu_1_2"
        ;;
        "xcvr") echo "port1 port2 port3 port4 port5 port6 port7 port8 port9 port10
             port11 port12 port13 port14 port15 port16 port17 port18 port19 port20
             port21 port22 port23 port24 port25 port26 port27 port28 port29 port30
             port31 port32"
        ;;
    esac
}

platform_do_power_operation() {
    local fru=$1
    local operation=$2
    local component=$3
    local ret=0

    if [ "$fru" = "psu" ]; then
        if [ "$operation" = "on" ] ||
           [ "$operation" = "off" ] ||
           [ "$operation" = "cycle" ]; then
            ipmitool power "$operation" > /dev/null
            ret=$?
        else
            ret=$STATUS_NOT_SUPPORTED
        fi
    elif [ "$fru" = "xcvr" ]; then
        port=$(printf "%02d" "${component/port/}")

        if [[ "$((10#$port))" -le 16 ]]; then
            pwrgd_path=$(realpath $CPLD1_SYSFS_DIR/qsfpdd_p"$port"_pwrgd)
            load_switch_path=$(realpath $CPLD1_SYSFS_DIR/en_p3v3_qsfpdd_p"$port")
            present_path=$(realpath $CPLD1_SYSFS_DIR/qsfpdd_p"$port"_present_n)
        else
            pwrgd_path=$(realpath $CPLD2_SYSFS_DIR/qsfpdd_p"$port"_pwrgd)
            load_switch_path=$(realpath $CPLD2_SYSFS_DIR/en_p3v3_qsfpdd_p"$port")
            present_path=$(realpath $CPLD2_SYSFS_DIR/qsfpdd_p"$port"_present_n)
        fi

        present=$(head -n1 $present_path 2> /dev/null)
        if [ $((present)) -ne 0 ]; then
            printf "\n"
            printf "%s : %s not present" "${fru^}" "$component"
            return "$STATUS_FAILURE"
        fi

        pwrgd=$(head -n1 $pwrgd_path 2> /dev/null)
        load_switch=$(head -n1 $load_switch_path 2> /dev/null)
        #shellcheck disable=SC2320
        if [ "$operation" = "on" ]; then
            if  [ $((pwrgd)) -eq 0 ] && [ $((load_switch)) -eq 1 ]; then
                echo 0 > "$load_switch_path"
            fi
            echo 1 > "$load_switch_path"
            ret=$?
        elif [ "$operation" = "off" ]; then
            if  [ $((pwrgd)) -eq 1 ] && [ $((load_switch)) -eq 0 ]; then
                echo 1 > "$load_switch_path"
            fi
            echo 0 > "$load_switch_path"
            ret=$?
        elif [ "$operation" = "cycle" ]; then
            echo 0 > "$load_switch_path"
            ret=$?
            sleep 1
            echo 1 > "$load_switch_path"
            ret=$((ret | $?))
        else
            ret=$STATUS_NOT_SUPPORTED
        fi
    else
         ret=$STATUS_NOT_SUPPORTED
    fi

    return "$ret"
}
