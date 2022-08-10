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

struct cpld_gpio_info *cpldinfo;

static int cpld_gpio_get(struct gpio_chip *gc, unsigned int offset)
{
  struct i2c_client *client  = gpiochip_get_data(gc);
  int ret;

  ret = (i2c_smbus_read_byte_data(client, cpldinfo[offset].reg) &
         BIT(cpldinfo[offset].shift)) >> cpldinfo[offset].shift;

  return ret;
}

static void cpld_gpio_set(struct gpio_chip *gc, unsigned int offset,
           int value)
{
  struct i2c_client *client = gpiochip_get_data(gc);
  int ret;

  ret = i2c_smbus_read_byte_data(client, cpldinfo[offset].reg);

  ret = value ? BIT(cpldinfo[offset].shift) | ret
              : ~(BIT(cpldinfo[offset].shift)) & ret;

  /* Page set not required */
  ret = i2c_smbus_write_byte_data(client, cpldinfo[offset].reg, ret);
  if (ret < 0) {
    dev_dbg(&client->dev, "Failed to write GPIO %d config: %d\n",
      offset, ret);
    return;
  }
}

static int cpld_gpio_set_direction(struct gpio_chip *gc,
              unsigned int offset, int direction_out)
{
  /* Direction should not be modified, so just return 0 to
     keep the original value. */
  return 0;
}

static int cpld_gpio_direction_input(struct gpio_chip *gc,
          unsigned int offset)
{
  return cpld_gpio_set_direction(gc, offset, 0);
}

static int cpld_gpio_direction_output(struct gpio_chip *gc,
           unsigned int offset, int val)
{
  return cpld_gpio_set_direction(gc, offset, 1);
}

static int cpld_gpio_get_direction(struct gpio_chip *gc,
              unsigned int offset)
{
  return cpldinfo[offset].direction;
}

static void cpld_probe_gpio(struct i2c_client *client,
             const int type,
             cpld_data *data)
{
  int rc;

  switch (type) {
    case cpu_cpld:
      data->gpio.ngpio = MAX_CPU_CPLD_GPIO;
      cpldinfo = cpu_cpld_gpios;
      break;
    case system_cpld0:
      data->gpio.ngpio = MAX_SYS_CPLD0_GPIO;
      cpldinfo = system_cpld0_gpios;
      break;
    case system_cpld1:
      data->gpio.ngpio = MAX_SYS_CPLD1_GPIO;
      cpldinfo = system_cpld1_gpios;
      break;
    case system_cpld2:
      data->gpio.ngpio = MAX_SYS_CPLD2_GPIO;
      cpldinfo = system_cpld2_gpios;
      break;
    case fan_cpld:
      data->gpio.ngpio = MAX_FAN_CPLD_GPIO;
      cpldinfo = fan_cpld_gpios;
      break;
    case fpga:
      data->gpio.ngpio = MAX_FPGA_GPIO;
      cpldinfo = fpga_gpios;
      break;
    default:
      return; /* GPIO support is optional. */
  }

  if (!data->gpio.ngpio)
    return;

  /*
   * Pinmux support has not been added to the new gpio_chip.
   * This support should be added when possible given the mux
   * behavior of these IO devices.
   */
  data->gpio.label = client->name;
  data->gpio.get_direction = cpld_gpio_get_direction;
  data->gpio.direction_input = cpld_gpio_direction_input;
  data->gpio.direction_output = cpld_gpio_direction_output;
  data->gpio.get = cpld_gpio_get;
  data->gpio.set = cpld_gpio_set;
  data->gpio.can_sleep = true;
  data->gpio.base = -1;
  data->gpio.parent = &client->dev;

  rc = devm_gpiochip_add_data(&client->dev, &data->gpio, client);
  if (rc)
    dev_warn(&client->dev, "Could not add gpiochip: %d\n", rc);
}

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

  cpld_probe_gpio(client, data->cpld_type, data);

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
