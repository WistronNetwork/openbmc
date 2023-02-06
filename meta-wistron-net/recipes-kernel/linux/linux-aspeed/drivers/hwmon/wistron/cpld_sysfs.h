/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * cpld_sysfs.h - Wistron CPLD sysfs library header
 *
 * Copyright (C) 2022 Neal Chen <neal_chen@wistron.com>
 */

#ifndef CPLD_SYSFS_H
#define CPLD_SYSFS_H

#include <linux/device.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/jiffies.h>
#include <linux/gpio/driver.h>
#include <linux/gpio.h>

struct cpld_gpio_info
{
  int reg;
  int shift;
  int direction;
};

typedef struct cpld_data_ {
  struct device *hwmon_dev;
  struct i2c_client *client;
  struct mutex update_lock;
  struct gpio_chip gpio;
  struct cpld_gpio_info *gpio_info;
  int cpld_type;
} cpld_data;

struct cpld_device_attribute {
  struct device_attribute dev_attr;
  int reg;
  int bit_offset;
  int n_bits;    /* number of bits */
};

#define to_cpld_dev_attr(_dev_attr) \
  container_of(_dev_attr, struct cpld_device_attribute, dev_attr)

#define CPLD_ATTR(_name, _mode, _show, _store, _reg, _bit_offset, _n_bits) \
  { .dev_attr = __ATTR(_name, _mode, _show, _store), \
    .reg = _reg,					     \
    .bit_offset = _bit_offset, \
    .n_bits =  _n_bits }

#define CPLD_DEVICE_ATTR(_name,_mode,_show,_store, _reg,_bit_offset,_n_bits) \
struct cpld_device_attribute cpld_dev_attr_##_name \
  = CPLD_ATTR(_name, _mode, _show, _store, _reg, _bit_offset, _n_bits)

#define CPLD_DEVICE_ATTR_RO(_name, _func, _reg, _bit_offset, _n_bits) \
  CPLD_DEVICE_ATTR(_name, 0444, _func##_show, NULL, \
                   _reg, _bit_offset, _n_bits)

#define CPLD_DEVICE_ATTR_RW(_name, _func, _reg, _bit_offset, _n_bits)	\
  CPLD_DEVICE_ATTR(_name, 0644, _func##_show, _func##_store,	\
                   _reg, _bit_offset, _n_bits)

#define CPLD_DEVICE_ATTR_WO(_name, _func, _reg, _bit_offset, _n_bits) \
  CPLD_DEVICE_ATTR(_name, 0200, NULL, _func##_store,		\
                   _reg, _bit_offset, _n_bits)

ssize_t cpld_n_bits_show(struct device *dev,
                         struct device_attribute *attr, char *buf);

ssize_t cpld_n_bits_store(struct device *dev,
                          struct device_attribute *attr, const char *buf,
                          size_t count);

#endif
