/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * wistron_cpld.c -  Wistron CPLD driver
 *
 * Copyright (C) 2022 Neal Chen <neal_chen@wistron.com>
 */

#include <linux/of_device.h>
#include "wistron_cpld.h"

enum chips {cpu_cpld, system_cpld0, system_cpld1, system_cpld2, fan_cpld, fpga};

ATTRIBUTE_GROUPS(cpu_cpld);
ATTRIBUTE_GROUPS(system_cpld0);
ATTRIBUTE_GROUPS(system_cpld1);
ATTRIBUTE_GROUPS(system_cpld2);
ATTRIBUTE_GROUPS(fan_cpld);
ATTRIBUTE_GROUPS(fpga);

static int wistron_cpld_probe(struct i2c_client *client,
                              const struct i2c_device_id *id)
{
  cpld_data *data;
  struct device *dev = &client->dev;
  int err;

  data = devm_kzalloc(&client->dev, sizeof(*data), GFP_KERNEL);
  if (!data)
    return -ENOMEM;

  i2c_set_clientdata(client, data);
  data->client = client;
  mutex_init(&data->update_lock);

  /* Set the device type */
  if (client->dev.of_node)
    data->cpld_type = (enum chips)of_device_get_match_data(&client->dev);
  else
    data->cpld_type = id->driver_data;

  switch (data->cpld_type) {
    case cpu_cpld:
      data->hwmon_dev = devm_hwmon_device_register_with_groups(dev,
                                    client->name, data, cpu_cpld_groups);
      break;
    case system_cpld0:
      data->hwmon_dev = devm_hwmon_device_register_with_groups(dev,
                                    client->name, data, system_cpld0_groups);
      break;
    case system_cpld1:
      data->hwmon_dev = devm_hwmon_device_register_with_groups(dev,
                                    client->name, data, system_cpld1_groups);
      break;
    case system_cpld2:
      data->hwmon_dev = devm_hwmon_device_register_with_groups(dev,
                                    client->name, data, system_cpld2_groups);
      break;
    case fan_cpld:
      data->hwmon_dev = devm_hwmon_device_register_with_groups(dev,
                                    client->name, data, fan_cpld_groups);
      break;
    case fpga:
      data->hwmon_dev = devm_hwmon_device_register_with_groups(dev,
                                    client->name, data, fpga_groups);
      break;
    default:
      break;
  }
  /* Everything is ready, now register the working device */
  if (IS_ERR(data->hwmon_dev)) {
    err = PTR_ERR(data->hwmon_dev);
    goto error_exit;
  }

  dev_info(dev, "Device '%s'\n", client->name);
  return 0;

error_exit:
  return err;
}

static int wistron_cpld_remove(struct i2c_client *client)
{
  cpld_data *data = i2c_get_clientdata(client);

  hwmon_device_unregister(data->hwmon_dev);

  return 0;
}

static const struct i2c_device_id wistron_cpld_id[] = {
  { "cpu_cpld", cpu_cpld },
  { "system_cpld0", system_cpld0 },
  { "system_cpld1", system_cpld1 },
  { "system_cpld2", system_cpld2 },
  { "fan_cpld", fan_cpld },
  { "fpga", fpga },
  { }
};
MODULE_DEVICE_TABLE(i2c, wistron_cpld_id);

static const struct of_device_id __maybe_unused wistron_cpld_of_match[] = {
  { .compatible = "wistron,cpu_cpld", .data = (void *)cpu_cpld },
  { .compatible = "wistron,system_cpld0", .data = (void *)system_cpld0 },
  { .compatible = "wistron,system_cpld1", .data = (void *)system_cpld1 },
  { .compatible = "wistron,system_cpld2", .data = (void *)system_cpld2 },
  { .compatible = "wistron,fan_cpld", .data = (void *)fan_cpld },
  { .compatible = "wistron,fpga", .data = (void *)fpga },
  { }
};
MODULE_DEVICE_TABLE(of, wistron_cpld_of_match);

static struct i2c_driver wistron_cpld_driver = {
  .class          = I2C_CLASS_HWMON,
  .driver         = {
    .name = "wistron_cpld",
    .of_match_table = wistron_cpld_of_match,
  },
  .probe          = wistron_cpld_probe,
  .remove         = wistron_cpld_remove,
  .id_table       = wistron_cpld_id,
};
module_i2c_driver(wistron_cpld_driver);

MODULE_AUTHOR("Neal Chen <neal_chen@wistron.com>");
MODULE_DESCRIPTION("Wistron CPLD driver");
MODULE_LICENSE("GPL");
