/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * wistron_cpld.h - Wistron CPLD driver header
 *
 * Copyright (C) 2022 Neal Chen <neal_chen@wistron.com>
 */

#ifndef WISTRON_CPLD_H
#define WISTRON_CPLD_H

#include "cpld_sysfs.h"

static struct attribute *cpu_cpld_attrs[] =  {
  NULL,
};

static struct attribute *system_cpld1_attrs[] =  {
  NULL,
};

static struct attribute *system_cpld2_attrs[] =  {
  NULL,
};

static struct attribute *fan_cpld_attrs[] =  {
  NULL,
};

#endif
