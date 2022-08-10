/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * wistron_cpld.h - Wistron CPLD driver header
 *
 * Copyright (C) 2022 Neal Chen <neal_chen@wistron.com>
 */

#ifndef WISTRON_CPLD_H
#define WISTRON_CPLD_H

#include "cpld_sysfs.h"

static struct attribute *cpu_cpld_attrs[] = {
  NULL,
};

static struct attribute *system_cpld0_attrs[] = {
  NULL,
};

static struct attribute *system_cpld1_attrs[] = {
  NULL,
};

static struct attribute *system_cpld2_attrs[] = {
  NULL,
};

static struct attribute *fan_cpld_attrs[] = {
  NULL,
};

static struct attribute *fpga_attrs[] = {
  NULL,
};

/** @brief struct cpld_gpio_info: mapping cpld information to
 *                                gpio offset parameter.
 *  member:
 *    reg - cpld register
 *    shift - bit offset of cpld register
 *    direction - 1: input, 0: output
 *
 *  @example
 *   static struct cpld_gpio_info cpu_cpld_gpios[] = {
 *   #define MAX_CPU_CPLD_GPIO 2
 *     [0] = {
 *       .reg = 0x0b,
 *       .shift = 3,
 *       .direction = 0
 *     },
 *     [1] = {
 *       .reg = 0x0c,
 *       .shift = 5,
 *       .direction = 1
 *     }
 *   }
 */

static struct cpld_gpio_info cpu_cpld_gpios[] = {
/* Define gpio numbers for CPU CPLD */
#define MAX_CPU_CPLD_GPIO 0
  NULL,
};

static struct cpld_gpio_info system_cpld0_gpios[] = {
/* Define gpio numbers for SYSTEM CPLD0 */
#define MAX_SYS_CPLD0_GPIO 0
  NULL,
};

static struct cpld_gpio_info system_cpld1_gpios[] = {
/* Define gpio numbers for SYSTEM CPLD1 */
#define MAX_SYS_CPLD1_GPIO 0
  NULL,
};

static struct cpld_gpio_info system_cpld2_gpios[] = {
/* Define gpio numbers for SYSTEM CPLD2 */
#define MAX_SYS_CPLD2_GPIO 0
  NULL,
};

static struct cpld_gpio_info fan_cpld_gpios[] = {
/* Define gpio numbers for FAN CPLD */
#define MAX_FAN_CPLD_GPIO 0
  NULL,
};

static struct cpld_gpio_info fpga_gpios[] = {
/* Define gpio numbers for FPGA */
#define MAX_FPGA_GPIO 0
  NULL,
};

#endif
