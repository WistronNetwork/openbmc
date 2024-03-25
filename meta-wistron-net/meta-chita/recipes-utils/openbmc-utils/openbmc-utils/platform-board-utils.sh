#!/bin/bash
#
# Copyright 2022-present Wistron. All Rights Reserved.
#

#shellcheck disable=SC1091
source /usr/local/bin/i2c-utils.sh

FANCPLD_SYSFS_DIR="$(i2c_device_sysfs_abspath 3-0016)/hwmon/hwmon*"
FAN_MAX=6

