#!/bin/bash
#
# Copyright 2022-present Wistron. All Rights Reserved.
#

STATUS_SUCCESS=0
STATUS_FAILURE=3
STATUS_NOT_SUPPORTED=4

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


##################### Fan function start #####################
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
            ((fan_rotor_id++))
            rpm_reg="$FANCPLD_SYSFS_DIR"/fan"$fan_rotor_id"_input
            rpm_f=$(head -n1 $rpm_reg 2> /dev/null)
            if [ "$((FAN_ROTOR))" -eq 2 ]; then
                ((fan_rotor_id++))
                rpm_reg="$FANCPLD_SYSFS_DIR"/fan"$fan_rotor_id"_input
                rpm_r=$(head -n1 $rpm_reg 2> /dev/null)
            fi
            pwm_reg="$FANCPLD_SYSFS_DIR"/pwm"$i"
            pwm=$(head -n1 $pwm_reg 2> /dev/null)
            if [ "$((FAN_ROTOR))" -eq 2 ]; then
                printf "Fan %s RPM: %s, %s, PWM: %s %%\n" "$i" "$rpm_f" "$rpm_r" "$((pwm))"
            else
                printf "Fan %s RPM: %s, PWM: %s %%\n" "$i" "$rpm_f" "$((pwm))"
            fi
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
##################### Fan function end #####################

##################### I2C/UART mux function start #####################
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
##################### I2C/UART mux function end #####################

bmc_model() {
    # Get BMC CPU information and SOC ID
    CPU_INFO=$(cat /proc/cpuinfo)
    if [[ $CPU_INFO = *ARMv7* ]]; then
        ID1=$(devmem 0x1e6e2004)
        ID2=$(devmem 0x1e6e2014)
        if [ $((ID1)) -eq $((0x05000303)) ] &&
           [ $((ID2)) -eq $((0x05000303)) ]; then
            echo "AST2600-A0"
        elif [ $((ID1)) -eq $((0x05010303)) ] &&
             [ $((ID2)) -eq $((0x05010303)) ]; then
            echo "AST2600-A1"
        elif [ $((ID1)) -eq $((0x05010303)) ] &&
             [ $((ID2)) -eq $((0x05020303)) ]; then
            echo "AST2600-A2"
        elif [ $((ID1)) -eq $((0x05030303)) ] &&
             [ $((ID2)) -eq $((0x05030303)) ]; then
            echo "AST2600-A3"
        elif [ $((ID1)) -eq $((0x05010203)) ] &&
             [ $((ID2)) -eq $((0x05010203)) ]; then
            echo "AST2620-A1"
        elif [ $((ID1)) -eq $((0x05010203)) ] &&
             [ $((ID2)) -eq $((0x05020203)) ]; then
            echo "AST2620-A2"
        elif [ $((ID1)) -eq $((0x05030203)) ] &&
             [ $((ID2)) -eq $((0x05030203)) ]; then
            echo "AST2620-A3"
        else
            echo "UNKNOWN"
        fi
    elif [[ $CPU_INFO = *ARMv6* ]]; then
        ID=$(devmem 0x1e6e207c)
        if [ $((ID)) -eq $((0x04000303)) ]; then
            echo "AST2500-A0"
        elif [ $((ID)) -eq $((0x04010303)) ]; then
            echo "AST2500-A1"
        elif [ $((ID)) -eq $((0x04030303)) ]; then
            echo "AST2500-A2"
        elif [ $((ID)) -eq $((0x04000203)) ]; then
            echo "AST2520-A1"
        elif [ $((ID)) -eq $((0x04010203)) ]; then
            echo "AST2520-A2"
        elif [ $((ID)) -eq $((0x04030203)) ]; then
            echo "AST2520-A3"
        else
            echo "UNKNOWN"
        fi
    else
        echo "UNKNOWN"
    fi
}

bmc_ready() {
    local operation=$1

    if declare -F platform_bmc_ready &> /dev/null; then
        platform_bmc_ready
    else
        if [ -z "$BMC_READY" ]; then
            printf "Not supported\n"
            return 2
        fi

        if [ ! -f "$BMC_READY_PATH" ]; then
            gpiocli -c 1e780000.gpio -n "$BMC_READY" -s BMC_READY export 2> /dev/null
        fi

        if [ "$operation" = "enable" ]; then
            if [ -f "$BMC_READY_PATH" ]; then
                echo "$BMC_READY_VALUE" > "$BMC_READY_PATH"
            else
                gpiocli -s BMC_READY set-init-value "$BMC_READY_VALUE" 2> /dev/null
            fi
            ret=$?
        elif [ "$operation" = "disable" ]; then
            if [ $((BMC_READY_VALUE)) -eq $((1)) ]; then
                value=0
            else
                value=1
            fi

            if [ -f "$BMC_READY_PATH" ]; then
                echo "$value" > "$I2C_MUX_SEL_PATH"
            else
                gpiocli -s BMC_READY set-init-value "$value" 2> /dev/null
            fi
            ret=$?
        else
            return 1
        fi

        if [ ! -f "$BMC_READY_PATH" ]; then
            gpiocli -s BMC_READY unexport 2> /dev/null
        fi

        return $ret
    fi
}

##################### FW function start #####################
fw_fru_list() {
    if declare -F platform_fw_fru_list &> /dev/null; then
        platform_fw_fru_list
    else
        echo "bmc"
    fi
}

fw_component_list() {
    local fru=$1

    if declare -F platform_fw_component_list &> /dev/null; then
        platform_fw_component_list "$fru"
    else
        if [ "$fru" = "bmc" ]; then
            echo "bmc altbmc"
        fi
    fi
}

is_valid_fw_fru() {
    local fru=$1

    fru_list=$(fw_fru_list)

    for f in $fru_list;
    do
        if [ "$f" = "$fru" ]; then
            echo "true"
            return
        fi
    done

    echo "false"
}

is_valid_fw_component() {
    local fru=$1
    local component=$2

    component_list=$(fw_component_list "$fru")

    for c in $component_list;
    do
        if [ "$c" = "$component" ]; then
            echo "true"
            return
        fi
    done

    echo "false"
}

print_fw_fru_comp() {
    fru_list=$(fw_fru_list)

    for fru in $fru_list;
    do
        component_list=$(fw_component_list "$fru")
        if [ -n "$component_list" ]; then
            printf "%-11s:" "$fru"
            printf " %s \n" "$component_list"
        fi
    done
}

get_mtd_name() {
    local bmc_label=$1

    mtd_name=$(cat < /proc/mtd | grep \""$bmc_label"\" | cut -d ":" -f 0)

    echo "$mtd_name"
}

do_bmc_version() {
    local component=$1

    if [ "$component" = "bmc" ]; then
        ver=$(cat < /etc/os-release | grep VERSION_ID= | cut -d "=" -f 2)
        ret=$STATUS_SUCCESS
    elif [ "$component" = "altbmc" ]; then
        altbmc_path="/run/media/rofs-alt/etc/os-release"
        if [ -f "$altbmc_path" ]; then
            ver=$(cat < $altbmc_path | grep VERSION_ID= | cut -d "=" -f 2)
            ret=$STATUS_SUCCESS
        else
            ver="Not Supported"
            ret=$STATUS_NOT_SUPPORTED
        fi
    else
        ver="Not Supported"
        ret=$STATUS_NOT_SUPPORTED
    fi
    echo "$ver"

    return "$ret"
}

do_bmc_update() {
    local component=$1
    local image=$2
    local force=$3

    boot_source=$(boot_info.sh bmc | grep "Boot Code Source" | cut -d ":" -f 2)
    extension=${image##*.}
    if [ "$component" = "bmc" ]; then
        if [[ $boot_source = *Slave* ]]; then
            printf "\nNot allow to update BMC backup flash when boot from backup flash\n"
            ret=$STATUS_NOT_SUPPORTED
        fi

        if [ "$extension" != "tar" ]; then
            printf "Invalid image format\n"
            return $STATUS_FAILURE
        fi
        cp "$image" /tmp/images && sleep 5
        #shellcheck disable=SC2012
        id=$(ls -t /tmp/images | head -n 1)

        busctl set-property xyz.openbmc_project.Software.BMC.Updater \
               /xyz/openbmc_project/software/"$id" \
               xyz.openbmc_project.Software.Activation RequestedActivation s \
               xyz.openbmc_project.Software.Activation.RequestedActivations.Active
        ret=$?

        if [ "$ret" -ne 0 ]; then
            ret=$STATUS_FAILURE
        else
            reboot
        fi
    elif [ "$component" = "altbmc" ]; then
        if [[ "$boot_source " = *Master* ]]; then
            if [ "$force" = "force" ]; then
                if [ "$extension" != "tar" ]; then
                    printf "Invalid image format\n"
                    return $STATUS_FAILURE
                fi
                tar -C /tmp -xvf "$image" &> /dev/null
                img_machine=$(cat < /tmp/MANIFEST | grep MachineName | cut -d "=" -f 2)
                machine=$(cat < /etc/os-release | grep -i machine | cut -d '"' -f 2)

                if [ "$img_machine" = "$machine" ]; then
                    alt_uboot_mtd=$(cat < /proc/mtd | grep \"alt-u-boot\" | cut -d ":" -f 0)
                    alt_kernel_mtd=$(cat < /proc/mtd | grep \"alt-kernel\" | cut -d ":" -f 0)
                    alt_rofs_mtd=$(cat < /proc/mtd | grep \"alt-rofs\" | cut -d ":" -f 0)

                    printf "Updating alt-kernel...\n"
                    flashcp -v /tmp/image-kernel /dev/"$alt_kernel_mtd"
                    ret=$?
                    printf "Updating alt-rofs...\n"
                    flashcp -v /tmp/image-rofs /dev/"$alt_rofs_mtd"
                    ret=$((ret | $?))
                    printf "Updating alt-u-boot...\n"
                    flashcp -v /tmp/image-u-boot /dev/"$alt_uboot_mtd"
                    ret=$((ret | $?))

                    rm /tmp/image-* &>/dev/null
                    rm /tmp/MANIFEST* &>/dev/null
                    rm "$image" &>/dev/null
                else
                    ret=$STATUS_FAILURE
                fi
            else
                printf "\nNot allow to update BMC backup flash\n"
                ret=$STATUS_NOT_SUPPORTED
            fi
        elif [[ "$boot_source " = *Slave* ]];then
            if [ "$image" = "recovery" ]; then
                uboot_mtd=$(cat < /proc/mtd | grep \"u-boot\" | cut -d ":" -f 0)
                kernel_mtd=$(cat < /proc/mtd | grep \"kernel\" | cut -d ":" -f 0)
                rofs_mtd=$(cat < /proc/mtd | grep \"rofs\" | cut -d ":" -f 0)
                alt_uboot_mtd=$(cat < /proc/mtd | grep \"alt-u-boot\" | cut -d ":" -f 0)
                alt_kernel_mtd=$(cat < /proc/mtd | grep \"alt-kernel\" | cut -d ":" -f 0)
                alt_rofs_mtd=$(cat < /proc/mtd | grep \"alt-rofs\" | cut -d ":" -f 0)

                cp /dev/"$uboot_mtd" /tmp/uboot && \
                cp /dev/"$kernel_mtd" /tmp/kernel && \
                cp /dev/"$rofs_mtd" /tmp/rofs

                printf "Recovery primary kernel...\n"
                flashcp -v /tmp/kernel /dev/"$alt_kernel_mtd"
                ret=$?
                printf "Recovery primary rofs...\n"
                flashcp -v /tmp/rofs /dev/"$alt_rofs_mtd"
                ret=$((ret | $?))
                printf "Recovery primary u-boot...\n"
                flashcp -v /tmp/uboot /dev/"$alt_uboot_mtd"
                ret=$((ret | $?))

                rm /tmp/uboot &>/dev/null
                rm /tmp/kernel &>/dev/null
                rm /tmp/rofs &>/dev/null
            else
                printf "\nNot allow to update BMC primary flash when boot from backup flash\n"
                ret=$STATUS_NOT_SUPPORTED
            fi
        else
            ret=1
        fi
    else
        ret=1
    fi

    return "$ret"
}

do_bmc_dump() {
    local component=$1
    local image=$2

    if [ "$component" = "bmc" ]; then
        label="bmc"
    elif [ "$component" = "altbmc" ]; then
        label="alt-bmc"
    else
        return 1
    fi

    mtd_name=$(get_mtd_name "$label")
    if [ "$mtd_name" = "" ]; then
        printf "Failed to get device for %s\n" "$label"
        return "$STATUS_FAILURE"
    fi

    dev_mtd="/dev/$mtd_name"
    printf "Dumping from device: %s\n" "$dev_mtd"
    dd if="$dev_mtd" of="$image"
    ret=$?

    if [ "$ret" -ne 0 ]; then
        ret=$STATUS_FAILURE
    fi

    return "$ret"
}

do_fw_version() {
    local fru=$1
    local component=$2
    local ver_ret=0

    if [ "$component" = "all" ]; then
        component=$(fw_component_list "$fru")
    fi

    for comp in $component;
    do
        if [ "$(is_valid_fw_component "$fru" "$comp")" = "true" ]; then
            if declare -F platform_do_"$fru"_version &> /dev/null; then
                ver=$(platform_do_"$fru"_version "$comp")
                ver_ret=$((ver_ret | $?))
            else
                if declare -F do_"$fru"_version &> /dev/null; then
                    ver=$(do_"$fru"_version "$comp")
                    ver_ret=$((ver_ret | $?))
                else
                    ver="Not Supported"
                    ver_ret=$STATUS_NOT_SUPPORTED
                fi
            fi
            printf "%s" "${comp^^} "
            printf "Version: %s\n" "$ver"
        else
            ver_ret=1
        fi
    done

    return $ver_ret
}

do_fw_update() {
    local fru=$1
    local component=$2
    local image=$3
    local force=$4
    local update_ret=0

    if [ "$(is_valid_fw_component "$fru" "$component")" = "true" ]; then
        start_time=$(date +%s)
        if declare -F platform_do_"$fru"_update &> /dev/null; then
            platform_do_"$fru"_update "$component" "$image" "$force"
            update_ret=$((update_ret | $?))
        else
            if declare -F do_"$fru"_update &> /dev/null; then
                do_"$fru"_update "$component" "$image" "$force"
                update_ret=$((update_ret | $?))

            else
                update_ret=$STATUS_NOT_SUPPORTED
            fi
        fi
        end_time=$(date +%s)
        total=$((end_time - start_time))

        printf "\n"
        printf "Upgrade of %s : %s" "$fru" "$component"
        if [ "$update_ret" -eq "$STATUS_SUCCESS" ]; then
            printf " succeeded\n"
        else
            if [ "$update_ret" -eq "$STATUS_NOT_SUPPORTED" ]; then
                printf " not supported\n"
            else
                printf " fail\n"
            fi
        fi

        printf "\n"
        printf "FW update time: %s senconds\n" "$total"
    else
        update_ret=1
    fi

    return $update_ret
}

do_fw_dump() {
    local fru=$1
    local component=$2
    local image=$3
    local dump_ret=0

    if [ "$(is_valid_fw_component "$fru" "$component")" = "true" ]; then
        if declare -F platform_do_"$fru"_dump &> /dev/null; then
            platform_do_"$fru"_dump "$component" "$image"
            dump_ret=$((dump_ret | $?))
        else
            if declare -F do_"$fru"_dump &> /dev/null; then
                do_"$fru"_dump "$component" "$image"
                dump_ret=$((dump_ret | $?))
            else
                dump_ret=$STATUS_NOT_SUPPORTED
            fi
        fi

        printf "\n"
        printf "Dump of %s : %s" "$fru" "$component"
        if [ "$dump_ret" -eq "$STATUS_SUCCESS" ]; then
            printf " succeeded\n"
        else
            if [ "$dump_ret" -eq "$STATUS_NOT_SUPPORTED" ]; then
                printf " not supported\n"
            else
                printf " fail\n"
            fi
        fi
    else
        dump_ret=1
    fi

    return $dump_ret
}
##################### FW function end #####################

##################### Power function start #####################
power_fru_list() {
    if declare -F platform_power_fru_list &> /dev/null; then
        platform_power_fru_list
    else
        echo "cpu"
    fi
}

power_component_list() {
    local fru=$1

    if declare -F platform_power_component_list &> /dev/null; then
        platform_power_component_list "$fru"
    else
        if [ "$fru" = "cpu" ]; then
            echo "cpu"
        fi
    fi
}

is_valid_power_fru() {
    local fru=$1

    fru_list=$(power_fru_list)

    for f in $fru_list;
    do
        if [ "$f" = "$fru" ]; then
            echo "true"
            return
        fi
    done

    echo "false"
}

is_valid_power_component() {
    local fru=$1
    local component=$2

    component_list=$(power_component_list "$fru")

    for c in $component_list;
    do
        if [ "$c" = "$component" ]; then
            echo "true"
            return
        fi
    done

    echo "false"
}

print_power_fru_comp() {
    fru_list=$(power_fru_list)

    for fru in $fru_list;
    do
        component_list=$(power_component_list "$fru")
        if [ -n "$component_list" ]; then
            printf "%-11s:" "$fru"
            printf " %s \n" "$component_list"
        fi
    done
}

do_power_operation() {
    local fru=$1
    local operation=$2
    local component=$3
    local ret=0

    if [ "$component" = "all" ]; then
        component=$(power_component_list "$fru")
    fi

    for comp in $component;
    do
        if [ "$(is_valid_power_component "$fru" "$comp")" = "true" ]; then
            if declare -F platform_do_power_operation &> /dev/null; then
                platform_do_power_operation "$fru" "$operation" "$comp"
                ret=$?
            else
                if [ "$fru" = "cpu" ] && [ "$comp" = "cpu" ]; then
                    ipmitool power "$operation" &> /dev/null
                    ret=$?
                else
                    ret="$STATUS_NOT_SUPPORTED"
                fi
            fi
            printf "\n"
            printf "Power %s of %s : %s" "$operation" "$fru" "$comp"
            if [ "$ret" -eq "$STATUS_SUCCESS" ]; then
                printf " succeeded\n"
            else
                if [ "$ret" -eq "$STATUS_NOT_SUPPORTED" ]; then
                    printf " not supported\n"
                else
                    printf " fail\n"
                fi
            fi
        else
            ret=1
        fi
    done

    return $ret
}
##################### Power function end #####################
