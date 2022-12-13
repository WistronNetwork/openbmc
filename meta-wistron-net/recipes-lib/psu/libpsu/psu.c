/*
 * Copyright 2022-present Wistron. All Rights Reserved.
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <openbmc/misc-utils.h>
#include "psu.h"

int psu_fan_command(const char* suid, uint8_t fan, enum fan_command_mode mode,
                    enum read_write operation, int *value) {
    char hwmon_path[PATH_MAX] = {0};
    char hwmon_path_real[PATH_MAX] = {0};
    char path[PATH_MAX] = {0};
    char str_value[10] = {0};
    int data;

    snprintf(hwmon_path, sizeof(hwmon_path), I2C_DEV_HWMON_DIR, suid);
    if (path_realpath(hwmon_path, hwmon_path_real))
        return -1;

    snprintf(path, sizeof(path), "%s/%s", hwmon_path_real, PSU_FAN_CONFIG_1_2);
    if (device_read(path, &data))
        return -1;

    if (mode == COMMAND_PWM && fan == 1) {
        data = data & (~0x40);
    } else if (mode == COMMAND_RPM && fan == 1) {
        data = data | 0x40;
    } else if (mode == COMMAND_PWM && fan == 2) {
        data = data & (~0x2);
    } else if (mode == COMMAND_RPM && fan == 2) {
        data = data | 0x2;
    } else {
        return -1;
    }

    sprintf(str_value, "%d", data);
    if (device_write_buff(path, str_value))
        return -1;

    if (fan == 1)
        snprintf(path, sizeof(path), "%s/%s",
                 hwmon_path_real, PSU_FAN_COMMAND1);
    else if (fan == 2)
        snprintf(path, sizeof(path), "%s/%s",
                 hwmon_path_real, PSU_FAN_COMMAND2);
    else
        return -1;

    if (operation == READ) {
        if (device_read(path, value))
            return -1;
    } else if (operation == WRITE){
        sprintf(str_value, "%d", *value & 0xff);
        if (device_write_buff(path, str_value))
            return -1;
    } else {
        return -1;
    }

    return 0;
}

int psu_block_read(const char* suid, uint8_t reg,
                   uint8_t data_len, char *data) {
    char hwmon_path[PATH_MAX] = {0};
    char hwmon_path_real[PATH_MAX] = {0};
    char path[PATH_MAX] = {0};
    char str_value[10] = {0};
    int fd;
    uint8_t ret_bytes;

    snprintf(hwmon_path, sizeof(hwmon_path), I2C_DEV_HWMON_DIR, suid);
    if (path_realpath(hwmon_path, hwmon_path_real))
        return -1;

    snprintf(path, sizeof(path), "%s/%s", hwmon_path_real, PSU_REG_LENGTH);
    snprintf(str_value, sizeof(str_value), "%d %d", reg, data_len);
    if (device_write_buff(path, str_value))
        return -1;

    snprintf(path, sizeof(path), "%s/%s", hwmon_path_real, PSU_BLOCK_READ);

    fd = open(path, O_RDONLY);
    if (fd < 0)
        return -1;

    ret_bytes = file_read_bytes(fd, data, data_len);
    if (ret_bytes != data_len) {
        close(fd);
        return -1;
    }
    close(fd);

    return 0;
}

int psu_status_read(const char* suid, uint8_t reg) {
    char hwmon_path[PATH_MAX] = {0};
    char hwmon_path_real[PATH_MAX] = {0};
    char path[PATH_MAX] = {0};
    char hwmon_num[10] = {0};
    int value;

    snprintf(hwmon_path, sizeof(hwmon_path), I2C_DEV_HWMON_DIR, suid);
    if (path_debugfs_hwmon(hwmon_path, hwmon_num))
        return -1;

    switch (reg) {
        case PMBUS_STATUS_WORD:
            snprintf(path, sizeof(path), PSU_STATUS0_WORD, hwmon_num);
            break;
        case PMBUS_STATUS_VOUT:
            snprintf(path, sizeof(path), PSU_STATUS0_WORD, hwmon_num);
            break;
        case PMBUS_STATUS_IOUT:
            snprintf(path, sizeof(path), PSU_STATUS0_IOUT, hwmon_num);
            break;
        case PMBUS_STATUS_INPUT:
            snprintf(path, sizeof(path), PSU_STATUS0_INPUT, hwmon_num);
            break;
        case PMBUS_STATUS_TEMPERATURE:
            snprintf(path, sizeof(path), PSU_STATUS0_TEMP, hwmon_num);
            break;
        case PMBUS_STATUS_CML:
            snprintf(path, sizeof(path), PSU_STATUS0_CML, hwmon_num);
            break;
        case PMBUS_STATUS_FAN_12:
            snprintf(path, sizeof(path), PSU_STATUS0_FAN_1_2, hwmon_num);
            break;
        case PMBUS_STATUS_MFR_SPECIFIC:
            if (path_realpath(hwmon_path, hwmon_path_real))
                return -1;
            snprintf(path, sizeof(path), "%s/%s",
                     hwmon_path_real, PSU_STATUS0_MFR);
            break;
        default:
            return -1;
    }

    if (device_read(path, &value))
        return -1;

    return value;
}
