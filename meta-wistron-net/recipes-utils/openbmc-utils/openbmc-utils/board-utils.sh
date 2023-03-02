#!/bin/bash
#
# Copyright 2022-present Wistron. All Rights Reserved.
#

#shellcheck disable=SC1091
if [ -f "/usr/local/bin/platform-board-utils.sh" ]; then
    source /usr/local/bin/platform-board-utils.sh
fi

bios_boot_info() {
    printf "Not supported\n"
}

bios_boot_from() {
    printf "Not supported\n"
}

# $1: Fan Unit
get_fan_speed() {
    if [ -z "$FANCPLD_SYSFS_DIR" ]; then
        printf "Not supported\n"
        return 2
    fi

    if [ $# -le 1 ]; then
        if [ $# -eq 1 ]; then
            if [ "$1" -lt 1 ] || [ "$1" -gt 7 ]; then
                return 1
            fi
            unit_start=$1
            unit_end=$1
        else
            unit_start=1
            unit_end=$FAN_MAX
        fi

        for ((i=unit_start; i<=unit_end; i++)); do
            rpm_reg="$FANCPLD_SYSFS_DIR"/fan"$i"_input
            rpm=$(head -n1 $rpm_reg 2> /dev/null)
            pwm_reg="$FANCPLD_SYSFS_DIR"/pwm"$i"
            pwm=$(head -n1 $pwm_reg 2> /dev/null)
            printf "Fan %s RPM: %s, PWM: %s %%\n" "$i" "$rpm" "$((pwm))"
        done

        return 0
    else
        return 1
    fi

}

# $1: PERCENT - 0..100
# $2: Fan Unit
set_fan_speed() {
    if [ -z "$FANCPLD_SYSFS_DIR" ]; then
        printf "Not supported\n"
        return 2
    fi

    if [ $# -eq 1 ] || [ $# -eq 2 ]; then
        if [ $# -eq 1 ]; then
            if [ "$1" -lt 0 ] || [ "$1" -gt 100 ]; then
                return 1
            fi
            unit_start=1
            unit_end=$FAN_MAX
        else
            if [ "$2" -lt 1 ] || [ "$2" -gt "$FAN_MAX" ]; then
                return 1
            fi
            unit_start=$2
            unit_end=$2
        fi

        step=$(($1))
        if [ $step -le 0 ]; then
            step=1
        fi

        for ((i=unit_start; i<=unit_end; i++)); do
            pwm_reg=$(realpath $FANCPLD_SYSFS_DIR/pwm"$i")
            echo "$step" > "$pwm_reg"
            printf "Set Fan %s speed to %s %%\n" "$i" "$1"
        done

        return 0
    else
        return 1
    fi
}

get_i2c_mux_master() {
    if [ -z "$I2C_MUX" ]; then
        printf "Not supported\n"
        return 2
    fi

    if [ -f "$I2C_MUX_SEL_PATH" ]; then
        mux=$(head -n 1 $I2C_MUX_SEL_PATH 2> /dev/null)
        ret=$?
    else
        gpiocli -c 1e780000.gpio -n "$I2C_MUX" -s "$I2C_MUX" export 2> /dev/null
        ret=$?
        mux=$(gpiocli -s "$I2C_MUX" get-value | cut -d "=" -f 2 2> /dev/null)
        gpiocli -s "$I2C_MUX" unexport 2> /dev/null
    fi

    if [ -z "$mux" ]; then
        printf "Unknown\n"
    fi

    if [ $((mux)) -eq "$I2C_MUX_CPU" ]; then
        printf "CPU\n"
    elif [ $((mux)) -eq "$I2C_MUX_BMC" ]; then
        printf "BMC\n"
    else
        printf "Unknown\n"
    fi

    if [ "$ret" -ne 0 ]; then
        return 3
    else
        return 0
    fi
}

get_uart_mux_master() {
    if [ -z "$UART_MUX" ]; then
        printf "Not supported\n"
        return 2
    fi

    mux=$(head -n1 $UART_MUX 2> /dev/null)
    ret=$?

    if [ -z "$mux" ]; then
        printf "Unknown\n"
    fi

    if [ $((mux)) -eq "$UART_MUX_BMC" ]; then
        printf "BMC\n"
    elif [ $((mux)) -eq "$UART_MUX_CPU" ]; then
        printf "CPU\n"
    else
        printf "Unknown\n"
    fi

    if [ "$ret" -ne 0 ]; then
        return 3
    else
        return 0
    fi
}

# $1: i2c or uart
get_mux_master() {
    if [ $# -eq 1 ]; then
        if [ "$1" = "i2c" ]; then
            if declare -F platform_get_i2c_mux_master &> /dev/null; then
                platform_get_i2c_mux_master
            else
                get_i2c_mux_master
            fi
            ret=$?
        elif [ "$1" = "uart" ]; then
            if declare -F platform_get_uart_mux_master &> /dev/null; then
                platform_get_uart_mux_master
            else
                get_uart_mux_master
            fi
            ret=$?
        else
            return 1
        fi
    else
        return 1
    fi

    return $ret
}

set_i2c_mux_master() {
    if [ -z "$I2C_MUX" ] || [ -z "$SENSOR_SERVICE" ]; then
        printf "Not supported\n"
        return 2
    fi



    if [ ! -f "$I2C_MUX_SEL_PATH" ]; then
        gpiocli -c 1e780000.gpio -n "$I2C_MUX" -s FM_MB_I2C_MUX_SEL export 2> /dev/null
    fi

    if [ "$1" = "cpu" ]; then
        systemctl stop "${SENSOR_SERVICE[@]}"
        sleep 0.5

        if [ -f "$I2C_MUX_SEL_PATH" ]; then
            echo "$I2C_MUX_CPU" > "$I2C_MUX_SEL_PATH"
        else
            gpiocli -s FM_MB_I2C_MUX_SEL set-init-value "$I2C_MUX_CPU" 2> /dev/null
        fi
        ret=$?
    elif [ "$1" = "bmc" ]; then
        if [ -f "$I2C_MUX_SEL_PATH" ]; then
            echo "$I2C_MUX_BMC" > "$I2C_MUX_SEL_PATH"
        else
            gpiocli -s FM_MB_I2C_MUX_SEL set-init-value "$I2C_MUX_BMC" 2> /dev/null
        fi
        ret=$?

        sleep 0.5
        systemctl start "${SENSOR_SERVICE[@]}" &
    else
        return 1
    fi

    if [ ! -f "$I2C_MUX_SEL_PATH" ]; then
        gpiocli -s FM_MB_I2C_MUX_SEL unexport 2> /dev/null
    fi

    return $ret
}

set_uart_mux_master() {
    if [ -z "$UART_MUX" ]; then
        printf "Not supported\n"
        return 2
    fi

    if [ "$1" = "cpu" ]; then
        value=$UART_MUX_CPU
    elif [ "$1" = "bmc" ]; then
        value=$UART_MUX_BMC
    else
        return 1
    fi

    echo "$value" > "$UART_MUX"
    ret=$?

    return $ret
}

# $1: i2c or uart
# $2: bmc or cpu
set_mux_master() {
    if [ $# -eq 2 ]; then
        if [ "$1" = "i2c" ]; then
            if declare -F platform_set_i2c_mux_master &> /dev/null; then
                platform_set_i2c_mux_master "$2"
            else
                set_i2c_mux_master "$2"
            fi
            ret=$?
        elif [ "$1" = "uart" ]; then
            if declare -F platform_set_uart_mux_master &> /dev/null; then
                platform_set_uart_mux_master "$2"
            else
                set_uart_mux_master "$2"
            fi
            ret=$?
        else
            return 1
        fi

        if [ "$ret" -eq 0 ]; then
            CONTROLLER=$1
            DEVICE=$2

            printf "Set %s Mux to %s\n" "${CONTROLLER^^}" "${DEVICE^^}"
        fi
    else
        return 1
    fi

    return $ret
}
