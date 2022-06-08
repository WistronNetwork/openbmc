/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * cpld_sysfs.c - Wistron CPLD sysfs library
 *
 * Copyright (C) 2022 Neal Chen <neal_chen@wistron.com>
 */

#include "cpld_sysfs.h"

ssize_t cpld_n_bits_show(struct device *dev,
                              struct device_attribute *attr, char *buf)
{
  struct cpld_device_attribute *cpld_attr = to_cpld_dev_attr(attr);
  cpld_data *data = dev_get_drvdata(dev);
  struct i2c_client *client = data->client;
  int reg_val;
  int val_mask;
  int val;

  val_mask = (1 << cpld_attr->n_bits) - 1;

  mutex_lock(&data->update_lock);
  reg_val = i2c_smbus_read_byte_data(client, cpld_attr->reg);
  mutex_unlock(&data->update_lock);

  if (reg_val < 0) {
    /* error case */
    return reg_val;
  }

  val = (reg_val >> cpld_attr->bit_offset) & val_mask;

  return sprintf(buf, "%#x\n", val);
}
EXPORT_SYMBOL_GPL(cpld_n_bits_show);

ssize_t cpld_n_bits_store(struct device *dev,
                                struct device_attribute *attr, const char *buf,
                                size_t count)
{
  struct cpld_device_attribute *cpld_attr = to_cpld_dev_attr(attr);
  cpld_data *data = dev_get_drvdata(dev);
  struct i2c_client *client = data->client;
  int reg_val;
  int val_mask;
  int val;

  if (sscanf(buf, "%i", &val) <= 0) {
    return -EINVAL;
  }

  val_mask = ~(((-1) >> cpld_attr->n_bits) << cpld_attr->n_bits);
  val &= val_mask;

  mutex_lock(&data->update_lock);

  reg_val = i2c_smbus_read_byte_data(client, cpld_attr->reg);
  if (reg_val < 0)
    goto exit_unlock;

  /* mask out all bits for the value requested */
  reg_val &= ~(val_mask << cpld_attr->bit_offset);
  reg_val |= val << cpld_attr->bit_offset;

  reg_val = i2c_smbus_write_byte_data(client, cpld_attr->reg, reg_val);

 exit_unlock:
  mutex_unlock(&data->update_lock);

  return reg_val < 0 ? reg_val : count;
}
EXPORT_SYMBOL_GPL(cpld_n_bits_store);

MODULE_AUTHOR("Neal Chen <neal_chen@wistron.com>");
MODULE_DESCRIPTION("Wistron CPLD read/write library");
MODULE_LICENSE("GPL");
