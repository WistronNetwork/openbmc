/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * sfphwmon.c - A simple SFP HWMON driver for temperature reading
 *
 * This driver is based on driver/net/phy/sfp.c
 *
 * Copyright (C) 2022 Neal Chen <neal_chen@wistron.com>
 */

#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/i2c.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/sfp.h>


typedef struct sfp_data_ {
  size_t i2c_block_size;
  struct sfp_diag diag;
  struct device *hwmon_dev;
  struct i2c_client *client;
  struct sfp_eeprom_id id;
  struct mutex update_lock;
} sfp_data;

static int sfphwmon_i2c_read(sfp_data *sfp, bool a2, u8 dev_addr, void *buf,
                             size_t len)
{
  struct i2c_adapter *adapter = sfp->client->adapter;
  struct i2c_msg msgs[2];
  u8 bus_addr = a2 ? 0x51 : 0x50;
  size_t block_size = sfp->i2c_block_size;
  size_t this_len;
  int ret;

  msgs[0].addr = bus_addr;
  msgs[0].flags = 0;
  msgs[0].len = 1;
  msgs[0].buf = &dev_addr;
  msgs[1].addr = bus_addr;
  msgs[1].flags = I2C_M_RD;
  msgs[1].len = len;
  msgs[1].buf = buf;

  mutex_lock(&sfp->update_lock);

  while (len) {
    this_len = len;
    if (this_len > block_size)
      this_len = block_size;

      msgs[1].len = this_len;

      ret = i2c_transfer(adapter, msgs, ARRAY_SIZE(msgs));
      if (ret < 0) {
        mutex_unlock(&sfp->update_lock);
        return ret;
      }

      if (ret != ARRAY_SIZE(msgs))
        break;

      msgs[1].buf += this_len;
      dev_addr += this_len;
      len -= this_len;
  }

  mutex_unlock(&sfp->update_lock);

  return msgs[1].buf - (u8 *)buf;
}

static umode_t sfphwmon_is_visible(const void *data,
                                   enum hwmon_sensor_types type,
                                   u32 attr, int channel)
{
  const sfp_data *sfp = data;

  switch (type) {
    case hwmon_temp:
      switch (attr) {
      case hwmon_temp_min_alarm:
      case hwmon_temp_max_alarm:
      case hwmon_temp_lcrit_alarm:
      case hwmon_temp_crit_alarm:
      case hwmon_temp_min:
      case hwmon_temp_max:
      case hwmon_temp_lcrit:
      case hwmon_temp_crit:
        if (!(sfp->id.ext.enhopts & SFP_ENHOPTS_ALARMWARN))
          return 0;
        fallthrough;
      case hwmon_temp_input:
      case hwmon_temp_label:
        return 0444;
      default:
        return 0;
      }
    default:
      return 0;
  }
}

static int sfphwmon_read_sensor(sfp_data *sfp, int reg, long *value)
{
  __be16 val;
  int err;

  err = sfphwmon_i2c_read(sfp, true, reg, &val, sizeof(val));
  if (err < 0)
    return err;

  *value = be16_to_cpu(val);

  return 0;
}

static void sfphwmon_calibrate(sfp_data *sfp, unsigned int slope,
                                     int offset, long *value)
{
  if (sfp->id.ext.diagmon & SFP_DIAGMON_EXT_CAL)
    *value = DIV_ROUND_CLOSEST(*value * slope, 256) + offset;
}

static void sfphwmon_calibrate_temp(sfp_data *sfp, long *value)
{
  sfphwmon_calibrate(sfp, be16_to_cpu(sfp->diag.cal_t_slope),
              be16_to_cpu(sfp->diag.cal_t_offset), value);

  if (*value >= 0x8000)
    *value -= 0x10000;

  *value = DIV_ROUND_CLOSEST(*value * 1000, 256);
}

static int sfphwmon_read_temp(sfp_data *sfp, int reg, long *value)
{
  int err;

  err = sfphwmon_read_sensor(sfp, reg, value);
  if (err < 0)
    return err;

  sfphwmon_calibrate_temp(sfp, value);

  return 0;
}

static int sfphwmon_temp(sfp_data *sfp, u32 attr, long *value)
{
  u8 status;
  int err;

  switch (attr) {
    case hwmon_temp_input:
      return sfphwmon_read_temp(sfp, SFP_TEMP, value);

    case hwmon_temp_lcrit:
      *value = be16_to_cpu(sfp->diag.temp_low_alarm);
      sfphwmon_calibrate_temp(sfp, value);
      return 0;

    case hwmon_temp_min:
      *value = be16_to_cpu(sfp->diag.temp_low_warn);
      sfphwmon_calibrate_temp(sfp, value);
      return 0;
    case hwmon_temp_max:
      *value = be16_to_cpu(sfp->diag.temp_high_warn);
      sfphwmon_calibrate_temp(sfp, value);
      return 0;

    case hwmon_temp_crit:
      *value = be16_to_cpu(sfp->diag.temp_high_alarm);
      sfphwmon_calibrate_temp(sfp, value);
      return 0;

    case hwmon_temp_lcrit_alarm:
      err = sfphwmon_i2c_read(sfp, true, SFP_ALARM0, &status, sizeof(status));
      if (err < 0)
        return err;

      *value = !!(status & SFP_ALARM0_TEMP_LOW);
      return 0;

    case hwmon_temp_min_alarm:
      err = sfphwmon_i2c_read(sfp, true, SFP_WARN0, &status, sizeof(status));
      if (err < 0)
        return err;

      *value = !!(status & SFP_WARN0_TEMP_LOW);
      return 0;

    case hwmon_temp_max_alarm:
      err = sfphwmon_i2c_read(sfp, true, SFP_WARN0, &status, sizeof(status));
      if (err < 0)
        return err;

      *value = !!(status & SFP_WARN0_TEMP_HIGH);
      return 0;

    case hwmon_temp_crit_alarm:
      err = sfphwmon_i2c_read(sfp, true, SFP_ALARM0, &status, sizeof(status));
      if (err < 0)
        return err;

      *value = !!(status & SFP_ALARM0_TEMP_HIGH);
      return 0;
    default:
      return -EOPNOTSUPP;
  }

  return -EOPNOTSUPP;
}

static int sfphwmon_read(struct device *dev, enum hwmon_sensor_types type,
        u32 attr, int channel, long *value)
{
  sfp_data *sfp = dev_get_drvdata(dev);

  switch (type) {
    case hwmon_temp:
      return sfphwmon_temp(sfp, attr, value);
    default:
      return -EOPNOTSUPP;
  }
}

static int sfphwmon_read_string(struct device *dev,
         enum hwmon_sensor_types type,
         u32 attr, int channel, const char **str)
{
  switch (type) {
    case hwmon_temp:
      switch (attr) {
      case hwmon_temp_label:
        *str = "temperature";
        return 0;
      default:
        return -EOPNOTSUPP;
      }
    default:
      return -EOPNOTSUPP;
  }

  return -EOPNOTSUPP;
}

static u32 sfphwmon_chip_config[] = {
	HWMON_C_REGISTER_TZ,
	0,
};

static const struct hwmon_channel_info sfphwmon_chip = {
	.type = hwmon_chip,
	.config = sfphwmon_chip_config,
};

static const struct hwmon_ops sfphwmon_ops = {
  .is_visible = sfphwmon_is_visible,
  .read = sfphwmon_read,
  .read_string = sfphwmon_read_string,
};

static u32 sfphwmon_temp_config[] = {
	HWMON_T_INPUT |
	HWMON_T_MAX | HWMON_T_MIN |
	HWMON_T_MAX_ALARM | HWMON_T_MIN_ALARM |
	HWMON_T_CRIT | HWMON_T_LCRIT |
	HWMON_T_CRIT_ALARM | HWMON_T_LCRIT_ALARM |
	HWMON_T_LABEL,
	0,
};

static const struct hwmon_channel_info sfphwmon_temp_channel_info = {
	.type = hwmon_temp,
	.config = sfphwmon_temp_config,
};

static const struct hwmon_channel_info *sfphwmon_info[] = {
	&sfphwmon_chip,
	&sfphwmon_temp_channel_info,
	NULL,
};

static const struct hwmon_chip_info sfphwmon_chip_info = {
	.ops = &sfphwmon_ops,
	.info = sfphwmon_info,
};

/* GPON modules based on Realtek RTL8672 and RTL9601C chips (e.g. V-SOL
 * V2801F, CarlitoxxPro CPGOS03-0490, Ubiquiti U-Fiber Instant, ...) do
 * not support multibyte reads from the EEPROM. Each multi-byte read
 * operation returns just one byte of EEPROM followed by zeros. There is
 * no way to identify which modules are using Realtek RTL8672 and RTL9601C
 * chips. Moreover every OEM of V-SOL V2801F module puts its own vendor
 * name and vendor id into EEPROM, so there is even no way to detect if
 * module is V-SOL V2801F. Therefore check for those zeros in the read
 * data and then based on check switch to reading EEPROM to one byte
 * at a time.
 */
static bool sfp_id_needs_byte_io(sfp_data *sfp, void *buf, size_t len)
{
  size_t i, block_size = sfp->i2c_block_size;

  /* Already using byte IO */
  if (block_size == 1)
    return false;

  for (i = 1; i < len; i += block_size) {
    if (memchr_inv(buf + i, '\0', min(block_size - 1, len - i)))
      return false;
  }
  return true;
}

static int sfphwmon_probe(struct i2c_client *client)
{
  struct i2c_adapter *adapter = client->adapter;
  struct device *dev = &client->dev;
  struct sfp_eeprom_id eeprom_id;
  sfp_data *sfp;
  int err;

  if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_WORD_DATA)) {
    dev_err(dev, "Adapter doesn't support SMBus word transactions\n");
    return -ENODEV;
  }

  sfp = devm_kzalloc(dev, sizeof(*sfp), GFP_KERNEL);
  if (!sfp) {
    dev_info(dev, "SFP can't alloc\n");
    return -ENOMEM;
  }

  i2c_set_clientdata(client, sfp);
  sfp->client = client;

  /* Some SFP modules and also some Linux I2C drivers do not like reads
   * longer than 16 bytes, so read the EEPROM in chunks of 16 bytes at
   * a time.
   */
  sfp->i2c_block_size = 16;

  err = sfphwmon_i2c_read(sfp, false, 0, &eeprom_id.base,
                          sizeof(eeprom_id.base));
  if (err < 0) {
    dev_err(dev, "Failed to read EEPROM: %pe\n", ERR_PTR(err));
    return -EAGAIN;
  }

  if (err != sizeof(eeprom_id.base)) {
    dev_err(dev, "EEPROM short read: %pe\n", ERR_PTR(err));
    return -EAGAIN;
  }

  /* Some SFP modules (e.g. Nokia 3FE46541AA) lock up if read from
   * address 0x51 is just one byte at a time. Also SFF-8472 requires
   * that EEPROM supports atomic 16bit read operation for diagnostic
   * fields, so do not switch to one byte reading at a time unless it
   * is really required and we have no other option.
   */
  if (sfp_id_needs_byte_io(sfp, &eeprom_id.base, sizeof(eeprom_id.base))) {
    dev_info(dev, "Detected broken RTL8672/RTL9601C emulated EEPROM\n");
    dev_info(dev, "Switching to reading EEPROM to one byte at a time\n");
    sfp->i2c_block_size = 1;

    err = sfphwmon_i2c_read(sfp, false, 0, &eeprom_id.base,
                            sizeof(eeprom_id.base));
    if (err < 0) {
      dev_err(dev, "failed to read EEPROM: %pe\n", ERR_PTR(err));
      return -EAGAIN;
    }

    if (err != sizeof(eeprom_id.base)) {
      dev_err(dev, "EEPROM short read: %pe\n", ERR_PTR(err));
      return -EAGAIN;
    }
  }

  err = sfphwmon_i2c_read(sfp, false, SFP_OPTIONS, &eeprom_id.ext,
                          sizeof(eeprom_id.ext));
  if (err < 0) {
    dev_err(dev, "failed to read EEPROM: %pe\n", ERR_PTR(err));
    return -EAGAIN;
  }

  if (err != sizeof(eeprom_id.ext)) {
    dev_err(dev, "EEPROM short read: %pe\n", ERR_PTR(err));
    return -EAGAIN;
  }

  sfp->id = eeprom_id;

  err = sfphwmon_i2c_read(sfp, true, 0, &sfp->diag, sizeof(sfp->diag));
  if (err < 0) {
      dev_warn(dev, "hwmon probe failed: %pe\n", ERR_PTR(err));
    return -EAGAIN;
  }

  mutex_init(&sfp->update_lock);

  sfp->hwmon_dev = devm_hwmon_device_register_with_info(dev, client->name,
                                            sfp, &sfphwmon_chip_info, NULL);

  if (IS_ERR(sfp->hwmon_dev)) {
    dev_err(dev, "unable to register sfp hwmon device\n");
    return PTR_ERR(sfp->hwmon_dev);
  }

  dev_info(dev, "Device '%s'\n", client->name);

  return 0;
}

static const struct i2c_device_id sfphwmon_id[] = {
  { "sfphwmon", 0 },
  { }
};
MODULE_DEVICE_TABLE(i2c, sfphwmon_id);

static const struct of_device_id __maybe_unused sfphwmon_of_match[] = {
  { .compatible = "sff,sfphwmon" },
  { }
};
MODULE_DEVICE_TABLE(of, sfphwmon_of_match);

static struct i2c_driver sfphwmon_driver = {
  .class          = I2C_CLASS_HWMON,
  .driver         = {
    .name = "sfphwmon",
    .of_match_table = of_match_ptr(sfphwmon_of_match),
  },
  .probe_new      = sfphwmon_probe,
  .id_table       = sfphwmon_id,
};
module_i2c_driver(sfphwmon_driver);

MODULE_AUTHOR("Neal Chen <neal_chen@wistron.com>");
MODULE_DESCRIPTION("A simple SFP HWMON driver");
MODULE_LICENSE("GPL");
