/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * wistron_cpld.h - Wistron CPLD driver header
 *
 * Copyright (C) 2022 Neal Chen <neal_chen@wistron.com>
 */

#ifndef WISTRON_CPLD_H
#define WISTRON_CPLD_H

#include "cpld_sysfs.h"

static ssize_t fan_rpm_show(struct device *dev,
                            struct device_attribute *attr, char *buf)
{
  struct cpld_device_attribute *cpld_attr = to_cpld_dev_attr(attr);
  cpld_data *data = dev_get_drvdata(dev);
  struct i2c_client *client = data->client;
  int val;

  mutex_lock(&data->update_lock);
  val = i2c_smbus_read_byte_data(client, cpld_attr->reg);
  mutex_unlock(&data->update_lock);

  return val < 0 ? val : sprintf(buf, "%u\n", val * 150);
}

static ssize_t fan_pwm_store(struct device *dev, struct device_attribute *attr,
                             const char *buf, size_t count)
{
  struct cpld_device_attribute *cpld_attr = to_cpld_dev_attr(attr);
  cpld_data *data = dev_get_drvdata(dev);
  struct i2c_client *client = data->client;
  int val;

  if (sscanf(buf, "%i", &val) <= 0)
    return -EINVAL;

  if (val >= 1 && val <= 100) {
    mutex_lock(&data->update_lock);
    val = i2c_smbus_write_byte_data(client, cpld_attr->reg, val - 1);
    mutex_unlock(&data->update_lock);
  } else {
    return -EINVAL;
  }

  return val < 0 ? val : count;
}

static ssize_t fan_pwm_show(struct device *dev,
                            struct device_attribute *attr, char *buf)
{
  struct cpld_device_attribute *cpld_attr = to_cpld_dev_attr(attr);
  cpld_data *data = dev_get_drvdata(dev);
  struct i2c_client *client = data->client;
  int val;

  mutex_lock(&data->update_lock);
  val = i2c_smbus_read_byte_data(client, cpld_attr->reg);
  mutex_unlock(&data->update_lock);

  return val < 0 ? val : sprintf(buf, "%u\n", val + 1);
}

static struct attribute *cpu_cpld_attrs[] = {
  NULL,
};

static struct attribute *system_cpld0_attrs[] =  {
  NULL,
};

enum cpld_regs {
  CPLD_VERSION        = 0x0,
  CPLD_RELEASE_DATE_1 = 0x1,
  CPLD_RELEASE_DATE_2 = 0x2,
  CPLD_BOARD_ID       = 0x3,
};

enum system_cpld1_regs {
  CPLD1_BMC_STATUS                    = 0x5,
  CPLD1_PSU_STATUS                    = 0x6,
  CPLD1_SYSTEM_POWER_CONTROL          = 0x7,
  CPLD1_SYSTEM_POWER_STATUS           = 0xe,
  CPLD1_RESET_STATUS                  = 0xf,
  CPLD1_THERMTRIP                     = 0x10,
  CPLD1_SYSTEM_INFO_CONTROL           = 0x11,
  CPLD1_RESET_DEVICE_CONTROL_1        = 0x12,
  CPLD1_RESET_DEVICE_CONTROL_2        = 0x13,
  CPLD1_BMC_RESET                     = 0x14,
  CPLD1_SYSTEM_LED_CONTROL_1          = 0x15,
  CPLD1_SYSTEM_LED_CONTROL_2          = 0x16,
  CPLD1_RESET_QDD_P01_P08_CONTROL     = 0x22,
  CPLD1_RESET_QDD_P09_P16_CONTROL     = 0x23,
  CPLD1_INT_QDD_P01_P08_STATUS        = 0x24,
  CPLD1_INT_QDD_P09_P16_STATUS        = 0x25,
  CPLD1_MODPRSNT_QDD_P01_P08_STATUS   = 0x26,
  CPLD1_MODPRSNT_QDD_P09_P16_STATUS   = 0x27,
  CPLD1_LPMODE_QDD_P01_P08_CONTROL    = 0x28,
  CPLD1_LPMODE_QDD_P09_P16_CONTROL    = 0x29,
  CPLD1_MODSEL_QDD_P01_P08_CONTROL    = 0x2a,
  CPLD1_MODSEL_QDD_P09_P16_CONTROL    = 0x2b,
  CPLD1_EN_P3V3_QDD_P01_P08_CONTROL   = 0x2c,
  CPLD1_TEST_P3V3_QDD_P01_P08_CONTROL = 0x2d,
  CPLD1_EN_P3V3_QDD_P09_P16_CONTROL   = 0x2e,
  CPLD1_TEST_P3V3_QDD_P09_P16_CONTROL = 0x2f,
  CPLD1_PWRGD_P3V3_QDD_P01_P08_STATUS = 0x30,
  CPLD1_PWRGD_P3V3_QDD_P09_P16_STATUS = 0x31,
  CPLD1_QDD_P01_P16_ALL_LED_CONTROL   = 0x33,
  CPLD1_QDD_P01_P02_LED_CONTROL       = 0x34,
  CPLD1_QDD_P03_P04_LED_CONTROL       = 0x35,
  CPLD1_QDD_P05_P06_LED_CONTROL       = 0x36,
  CPLD1_QDD_P07_P08_LED_CONTROL       = 0x37,
  CPLD1_QDD_P09_P10_LED_CONTROL       = 0x38,
  CPLD1_QDD_P11_P12_LED_CONTROL       = 0x39,
  CPLD1_QDD_P13_P14_LED_CONTROL       = 0x3a,
  CPLD1_QDD_P15_P16_LED_CONTROL       = 0x3b,
  CPLD1_GPIO                          = 0x3c,
  CPLD1_DEBUG_GPIO                    = 0x3d,
  CPLD1_DEBUG_LED                     = 0x3e,
  CPLD1_INTERRUPT_STATUS_1            = 0x3f,
  CPLD1_INTERRUPT_STATUS_2            = 0x40,
  CPLD1_INTERRUPT_STATUS_3            = 0x41,
  CPLD1_QDD_P01_P08_INT_INSERT_STATUS = 0x42,
  CPLD1_QDD_P09_P16_INT_INSERT_STATUS = 0x43,
};

static CPLD_DEVICE_ATTR_RO(cpld1_minor_version, cpld_n_bits, CPLD_VERSION, 0, 4);
static CPLD_DEVICE_ATTR_RO(cpld1_major_version, cpld_n_bits, CPLD_VERSION, 4, 4);
static CPLD_DEVICE_ATTR_RO(cpld1_release_month, cpld_n_bits, CPLD_RELEASE_DATE_1, 0, 4);
static CPLD_DEVICE_ATTR_RO(cpld1_release_year, cpld_n_bits, CPLD_RELEASE_DATE_1, 4, 4);
static CPLD_DEVICE_ATTR_RO(cpld1_release_day, cpld_n_bits, CPLD_RELEASE_DATE_2, 0, 4);
static CPLD_DEVICE_ATTR_RO(cpld1_build_rev, cpld_n_bits, CPLD_BOARD_ID, 0, 2);
static CPLD_DEVICE_ATTR_RO(cpld1_hw_rev, cpld_n_bits, CPLD_BOARD_ID, 2, 2);
static CPLD_DEVICE_ATTR_RO(cpld1_board_id, cpld_n_bits, CPLD_BOARD_ID, 4, 4);
static CPLD_DEVICE_ATTR_RO(psu1_smbalert_interrupt_n, cpld_n_bits, CPLD1_PSU_STATUS, 0, 1);
static CPLD_DEVICE_ATTR_RO(psu1_present_n, cpld_n_bits, CPLD1_PSU_STATUS, 1, 1);
static CPLD_DEVICE_ATTR_RO(psu1_pwrgd, cpld_n_bits, CPLD1_PSU_STATUS, 2, 1);
static CPLD_DEVICE_ATTR_RO(psu1_acok, cpld_n_bits, CPLD1_PSU_STATUS, 3, 1);
static CPLD_DEVICE_ATTR_RO(psu2_smbalert_interrupt_n, cpld_n_bits, CPLD1_PSU_STATUS, 4, 1);
static CPLD_DEVICE_ATTR_RO(psu2_present_n, cpld_n_bits, CPLD1_PSU_STATUS, 5, 1);
static CPLD_DEVICE_ATTR_RO(psu2_pwrgd, cpld_n_bits, CPLD1_PSU_STATUS, 6, 1);
static CPLD_DEVICE_ATTR_RO(psu2_acok, cpld_n_bits, CPLD1_PSU_STATUS, 7, 1);
static CPLD_DEVICE_ATTR_RO(psu_on_n, cpld_n_bits, CPLD1_SYSTEM_POWER_CONTROL, 0, 1);
static CPLD_DEVICE_ATTR_RW(psu1_pson_n, cpld_n_bits, CPLD1_SYSTEM_POWER_CONTROL, 1, 1);
static CPLD_DEVICE_ATTR_RW(psu2_pson_n, cpld_n_bits, CPLD1_SYSTEM_POWER_CONTROL, 2, 1);
static CPLD_DEVICE_ATTR_RW(p3v3_b_en, cpld_n_bits, CPLD1_SYSTEM_POWER_CONTROL, 4, 1);
static CPLD_DEVICE_ATTR_RW(p3v3_a_en, cpld_n_bits, CPLD1_SYSTEM_POWER_CONTROL, 5, 1);
static CPLD_DEVICE_ATTR_RO(bmc_pwrgd, cpld_n_bits, CPLD1_SYSTEM_POWER_STATUS, 0, 1);
static CPLD_DEVICE_ATTR_RO(psu_pwrgd, cpld_n_bits, CPLD1_SYSTEM_POWER_STATUS, 1, 1);
static CPLD_DEVICE_ATTR_RO(p3v3_b_pwrgd, cpld_n_bits, CPLD1_SYSTEM_POWER_STATUS, 3, 1);
static CPLD_DEVICE_ATTR_RO(p3v3_a_pwrgd, cpld_n_bits, CPLD1_SYSTEM_POWER_STATUS, 4, 1);
static CPLD_DEVICE_ATTR_RO(system_pwrgd, cpld_n_bits, CPLD1_SYSTEM_POWER_STATUS, 5, 1);
static CPLD_DEVICE_ATTR_RO(cpld1_rst_platrst_cpld1_n, cpld_n_bits, CPLD1_RESET_STATUS, 0, 1);
static CPLD_DEVICE_ATTR_RO(cpld1_rst_swrst_cpld1_n, cpld_n_bits, CPLD1_RESET_STATUS, 1, 1);
static CPLD_DEVICE_ATTR_RO(cpld1_cpld2_thermtrip_n, cpld_n_bits, CPLD1_THERMTRIP, 0, 1);
static CPLD_DEVICE_ATTR_RO(cpld1_thermtrip_n, cpld_n_bits, CPLD1_THERMTRIP, 1, 1);
static CPLD_DEVICE_ATTR_RO(en_cpld1_hitless_n, cpld_n_bits, CPLD1_SYSTEM_INFO_CONTROL, 0, 1);
static CPLD_DEVICE_ATTR_RW(mb_eeprom_wc_n, cpld_n_bits, CPLD1_SYSTEM_INFO_CONTROL, 2, 1);
static CPLD_DEVICE_ATTR_RW(rst_fancpld_n, cpld_n_bits, CPLD1_RESET_DEVICE_CONTROL_1, 0, 1);
static CPLD_DEVICE_ATTR_RW(rst_phy_n, cpld_n_bits, CPLD1_RESET_DEVICE_CONTROL_1, 1, 1);
static CPLD_DEVICE_ATTR_RW(rst_i2c_mux_d0_n, cpld_n_bits, CPLD1_RESET_DEVICE_CONTROL_1, 4, 1);
static CPLD_DEVICE_ATTR_RW(rst_i2c_mux_d1_n, cpld_n_bits, CPLD1_RESET_DEVICE_CONTROL_2, 0, 1);
static CPLD_DEVICE_ATTR_RW(rst_i2c_mux_d2_n, cpld_n_bits, CPLD1_RESET_DEVICE_CONTROL_2, 1, 1);
static CPLD_DEVICE_ATTR_RW(rst_i2c_mux_d3_n, cpld_n_bits, CPLD1_RESET_DEVICE_CONTROL_2, 2, 1);
static CPLD_DEVICE_ATTR_RW(rst_i2c_mux_d4_n, cpld_n_bits, CPLD1_RESET_DEVICE_CONTROL_2, 3, 1);
static CPLD_DEVICE_ATTR_RW(rst_i2c_mux_d5_n, cpld_n_bits, CPLD1_RESET_DEVICE_CONTROL_2, 4, 1);
static CPLD_DEVICE_ATTR_RW(rst_i2c_mux_d6_n, cpld_n_bits, CPLD1_RESET_DEVICE_CONTROL_2, 5, 1);
static CPLD_DEVICE_ATTR_RW(rst_i2c_mux_d7_n, cpld_n_bits, CPLD1_RESET_DEVICE_CONTROL_2, 6, 1);
static CPLD_DEVICE_ATTR_RW(rst_i2c_mux_d8_n, cpld_n_bits, CPLD1_RESET_DEVICE_CONTROL_2, 7, 1);
static CPLD_DEVICE_ATTR_RW(sys_led_control, cpld_n_bits, CPLD1_SYSTEM_LED_CONTROL_1, 0, 3);
static CPLD_DEVICE_ATTR_RW(psu_led_control, cpld_n_bits, CPLD1_SYSTEM_LED_CONTROL_1, 4, 3);
static CPLD_DEVICE_ATTR_RW(fan_led_control, cpld_n_bits, CPLD1_SYSTEM_LED_CONTROL_2, 0, 3);
static CPLD_DEVICE_ATTR_RW(fault_led_control, cpld_n_bits, CPLD1_SYSTEM_LED_CONTROL_2, 4, 3);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p01_n, cpld_n_bits, CPLD1_RESET_QDD_P01_P08_CONTROL, 0, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p02_n, cpld_n_bits, CPLD1_RESET_QDD_P01_P08_CONTROL, 1, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p03_n, cpld_n_bits, CPLD1_RESET_QDD_P01_P08_CONTROL, 2, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p04_n, cpld_n_bits, CPLD1_RESET_QDD_P01_P08_CONTROL, 3, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p05_n, cpld_n_bits, CPLD1_RESET_QDD_P01_P08_CONTROL, 4, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p06_n, cpld_n_bits, CPLD1_RESET_QDD_P01_P08_CONTROL, 5, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p07_n, cpld_n_bits, CPLD1_RESET_QDD_P01_P08_CONTROL, 6, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p08_n, cpld_n_bits, CPLD1_RESET_QDD_P01_P08_CONTROL, 7, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p09_n, cpld_n_bits, CPLD1_RESET_QDD_P09_P16_CONTROL, 0, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p10_n, cpld_n_bits, CPLD1_RESET_QDD_P09_P16_CONTROL, 1, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p11_n, cpld_n_bits, CPLD1_RESET_QDD_P09_P16_CONTROL, 2, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p12_n, cpld_n_bits, CPLD1_RESET_QDD_P09_P16_CONTROL, 3, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p13_n, cpld_n_bits, CPLD1_RESET_QDD_P09_P16_CONTROL, 4, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p14_n, cpld_n_bits, CPLD1_RESET_QDD_P09_P16_CONTROL, 5, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p15_n, cpld_n_bits, CPLD1_RESET_QDD_P09_P16_CONTROL, 6, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p16_n, cpld_n_bits, CPLD1_RESET_QDD_P09_P16_CONTROL, 7, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p01_interrupt_n, cpld_n_bits, CPLD1_INT_QDD_P01_P08_STATUS, 0, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p02_interrupt_n, cpld_n_bits, CPLD1_INT_QDD_P01_P08_STATUS, 1, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p03_interrupt_n, cpld_n_bits, CPLD1_INT_QDD_P01_P08_STATUS, 2, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p04_interrupt_n, cpld_n_bits, CPLD1_INT_QDD_P01_P08_STATUS, 3, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p05_interrupt_n, cpld_n_bits, CPLD1_INT_QDD_P01_P08_STATUS, 4, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p06_interrupt_n, cpld_n_bits, CPLD1_INT_QDD_P01_P08_STATUS, 5, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p07_interrupt_n, cpld_n_bits, CPLD1_INT_QDD_P01_P08_STATUS, 6, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p08_interrupt_n, cpld_n_bits, CPLD1_INT_QDD_P01_P08_STATUS, 7, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p01_p08_interrupt_n, cpld_n_bits, CPLD1_INT_QDD_P01_P08_STATUS, 0, 8);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p09_interrupt_n, cpld_n_bits, CPLD1_INT_QDD_P09_P16_STATUS, 0, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p10_interrupt_n, cpld_n_bits, CPLD1_INT_QDD_P09_P16_STATUS, 1, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p11_interrupt_n, cpld_n_bits, CPLD1_INT_QDD_P09_P16_STATUS, 2, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p12_interrupt_n, cpld_n_bits, CPLD1_INT_QDD_P09_P16_STATUS, 3, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p13_interrupt_n, cpld_n_bits, CPLD1_INT_QDD_P09_P16_STATUS, 4, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p14_interrupt_n, cpld_n_bits, CPLD1_INT_QDD_P09_P16_STATUS, 5, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p15_interrupt_n, cpld_n_bits, CPLD1_INT_QDD_P09_P16_STATUS, 6, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p16_interrupt_n, cpld_n_bits, CPLD1_INT_QDD_P09_P16_STATUS, 7, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p09_p16_interrupt_n, cpld_n_bits, CPLD1_INT_QDD_P09_P16_STATUS, 0, 8);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p01_p16_interrupt_n, cpld_n_bits, CPLD1_INTERRUPT_STATUS_1, 1, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p01_present_n, cpld_n_bits, CPLD1_MODPRSNT_QDD_P01_P08_STATUS, 0, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p02_present_n, cpld_n_bits, CPLD1_MODPRSNT_QDD_P01_P08_STATUS, 1, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p03_present_n, cpld_n_bits, CPLD1_MODPRSNT_QDD_P01_P08_STATUS, 2, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p04_present_n, cpld_n_bits, CPLD1_MODPRSNT_QDD_P01_P08_STATUS, 3, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p05_present_n, cpld_n_bits, CPLD1_MODPRSNT_QDD_P01_P08_STATUS, 4, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p06_present_n, cpld_n_bits, CPLD1_MODPRSNT_QDD_P01_P08_STATUS, 5, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p07_present_n, cpld_n_bits, CPLD1_MODPRSNT_QDD_P01_P08_STATUS, 6, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p08_present_n, cpld_n_bits, CPLD1_MODPRSNT_QDD_P01_P08_STATUS, 7, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p01_p08_present_n, cpld_n_bits, CPLD1_MODPRSNT_QDD_P01_P08_STATUS, 0, 8);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p09_present_n, cpld_n_bits, CPLD1_MODPRSNT_QDD_P09_P16_STATUS, 0, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p10_present_n, cpld_n_bits, CPLD1_MODPRSNT_QDD_P09_P16_STATUS, 1, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p11_present_n, cpld_n_bits, CPLD1_MODPRSNT_QDD_P09_P16_STATUS, 2, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p12_present_n, cpld_n_bits, CPLD1_MODPRSNT_QDD_P09_P16_STATUS, 3, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p13_present_n, cpld_n_bits, CPLD1_MODPRSNT_QDD_P09_P16_STATUS, 4, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p14_present_n, cpld_n_bits, CPLD1_MODPRSNT_QDD_P09_P16_STATUS, 5, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p15_present_n, cpld_n_bits, CPLD1_MODPRSNT_QDD_P09_P16_STATUS, 6, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p16_present_n, cpld_n_bits, CPLD1_MODPRSNT_QDD_P09_P16_STATUS, 7, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p09_p16_present_n, cpld_n_bits, CPLD1_MODPRSNT_QDD_P09_P16_STATUS, 0, 8);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p01, cpld_n_bits, CPLD1_LPMODE_QDD_P01_P08_CONTROL, 0, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p02, cpld_n_bits, CPLD1_LPMODE_QDD_P01_P08_CONTROL, 1, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p03, cpld_n_bits, CPLD1_LPMODE_QDD_P01_P08_CONTROL, 2, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p04, cpld_n_bits, CPLD1_LPMODE_QDD_P01_P08_CONTROL, 3, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p05, cpld_n_bits, CPLD1_LPMODE_QDD_P01_P08_CONTROL, 4, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p06, cpld_n_bits, CPLD1_LPMODE_QDD_P01_P08_CONTROL, 5, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p07, cpld_n_bits, CPLD1_LPMODE_QDD_P01_P08_CONTROL, 6, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p08, cpld_n_bits, CPLD1_LPMODE_QDD_P01_P08_CONTROL, 7, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p09, cpld_n_bits, CPLD1_LPMODE_QDD_P09_P16_CONTROL, 0, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p10, cpld_n_bits, CPLD1_LPMODE_QDD_P09_P16_CONTROL, 1, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p11, cpld_n_bits, CPLD1_LPMODE_QDD_P09_P16_CONTROL, 2, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p12, cpld_n_bits, CPLD1_LPMODE_QDD_P09_P16_CONTROL, 3, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p13, cpld_n_bits, CPLD1_LPMODE_QDD_P09_P16_CONTROL, 4, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p14, cpld_n_bits, CPLD1_LPMODE_QDD_P09_P16_CONTROL, 5, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p15, cpld_n_bits, CPLD1_LPMODE_QDD_P09_P16_CONTROL, 6, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p16, cpld_n_bits, CPLD1_LPMODE_QDD_P09_P16_CONTROL, 7, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p01_n, cpld_n_bits, CPLD1_MODSEL_QDD_P01_P08_CONTROL, 0, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p02_n, cpld_n_bits, CPLD1_MODSEL_QDD_P01_P08_CONTROL, 1, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p03_n, cpld_n_bits, CPLD1_MODSEL_QDD_P01_P08_CONTROL, 2, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p04_n, cpld_n_bits, CPLD1_MODSEL_QDD_P01_P08_CONTROL, 3, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p05_n, cpld_n_bits, CPLD1_MODSEL_QDD_P01_P08_CONTROL, 4, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p06_n, cpld_n_bits, CPLD1_MODSEL_QDD_P01_P08_CONTROL, 5, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p07_n, cpld_n_bits, CPLD1_MODSEL_QDD_P01_P08_CONTROL, 6, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p08_n, cpld_n_bits, CPLD1_MODSEL_QDD_P01_P08_CONTROL, 7, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p09_n, cpld_n_bits, CPLD1_MODSEL_QDD_P09_P16_CONTROL, 0, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p10_n, cpld_n_bits, CPLD1_MODSEL_QDD_P09_P16_CONTROL, 1, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p11_n, cpld_n_bits, CPLD1_MODSEL_QDD_P09_P16_CONTROL, 2, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p12_n, cpld_n_bits, CPLD1_MODSEL_QDD_P09_P16_CONTROL, 3, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p13_n, cpld_n_bits, CPLD1_MODSEL_QDD_P09_P16_CONTROL, 4, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p14_n, cpld_n_bits, CPLD1_MODSEL_QDD_P09_P16_CONTROL, 5, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p15_n, cpld_n_bits, CPLD1_MODSEL_QDD_P09_P16_CONTROL, 6, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p16_n, cpld_n_bits, CPLD1_MODSEL_QDD_P09_P16_CONTROL, 7, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p01, cpld_n_bits, CPLD1_EN_P3V3_QDD_P01_P08_CONTROL, 0, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p02, cpld_n_bits, CPLD1_EN_P3V3_QDD_P01_P08_CONTROL, 1, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p03, cpld_n_bits, CPLD1_EN_P3V3_QDD_P01_P08_CONTROL, 2, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p04, cpld_n_bits, CPLD1_EN_P3V3_QDD_P01_P08_CONTROL, 3, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p05, cpld_n_bits, CPLD1_EN_P3V3_QDD_P01_P08_CONTROL, 4, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p06, cpld_n_bits, CPLD1_EN_P3V3_QDD_P01_P08_CONTROL, 5, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p07, cpld_n_bits, CPLD1_EN_P3V3_QDD_P01_P08_CONTROL, 6, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p08, cpld_n_bits, CPLD1_EN_P3V3_QDD_P01_P08_CONTROL, 7, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p09, cpld_n_bits, CPLD1_EN_P3V3_QDD_P09_P16_CONTROL, 0, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p10, cpld_n_bits, CPLD1_EN_P3V3_QDD_P09_P16_CONTROL, 1, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p11, cpld_n_bits, CPLD1_EN_P3V3_QDD_P09_P16_CONTROL, 2, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p12, cpld_n_bits, CPLD1_EN_P3V3_QDD_P09_P16_CONTROL, 3, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p13, cpld_n_bits, CPLD1_EN_P3V3_QDD_P09_P16_CONTROL, 4, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p14, cpld_n_bits, CPLD1_EN_P3V3_QDD_P09_P16_CONTROL, 5, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p15, cpld_n_bits, CPLD1_EN_P3V3_QDD_P09_P16_CONTROL, 6, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p16, cpld_n_bits, CPLD1_EN_P3V3_QDD_P09_P16_CONTROL, 7, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p01_pwrgd, cpld_n_bits, CPLD1_PWRGD_P3V3_QDD_P01_P08_STATUS, 0, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p02_pwrgd, cpld_n_bits, CPLD1_PWRGD_P3V3_QDD_P01_P08_STATUS, 1, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p03_pwrgd, cpld_n_bits, CPLD1_PWRGD_P3V3_QDD_P01_P08_STATUS, 2, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p04_pwrgd, cpld_n_bits, CPLD1_PWRGD_P3V3_QDD_P01_P08_STATUS, 3, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p05_pwrgd, cpld_n_bits, CPLD1_PWRGD_P3V3_QDD_P01_P08_STATUS, 4, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p06_pwrgd, cpld_n_bits, CPLD1_PWRGD_P3V3_QDD_P01_P08_STATUS, 5, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p07_pwrgd, cpld_n_bits, CPLD1_PWRGD_P3V3_QDD_P01_P08_STATUS, 6, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p08_pwrgd, cpld_n_bits, CPLD1_PWRGD_P3V3_QDD_P01_P08_STATUS, 7, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p09_pwrgd, cpld_n_bits, CPLD1_PWRGD_P3V3_QDD_P09_P16_STATUS, 0, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p10_pwrgd, cpld_n_bits, CPLD1_PWRGD_P3V3_QDD_P09_P16_STATUS, 1, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p11_pwrgd, cpld_n_bits, CPLD1_PWRGD_P3V3_QDD_P09_P16_STATUS, 2, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p12_pwrgd, cpld_n_bits, CPLD1_PWRGD_P3V3_QDD_P09_P16_STATUS, 3, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p13_pwrgd, cpld_n_bits, CPLD1_PWRGD_P3V3_QDD_P09_P16_STATUS, 4, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p14_pwrgd, cpld_n_bits, CPLD1_PWRGD_P3V3_QDD_P09_P16_STATUS, 5, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p15_pwrgd, cpld_n_bits, CPLD1_PWRGD_P3V3_QDD_P09_P16_STATUS, 6, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p16_pwrgd, cpld_n_bits, CPLD1_PWRGD_P3V3_QDD_P09_P16_STATUS, 7, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p01_p16_pwrgd_interrupt_n, cpld_n_bits, CPLD1_INTERRUPT_STATUS_1, 2, 1);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p01_p16_led_control, cpld_n_bits, CPLD1_QDD_P01_P16_ALL_LED_CONTROL, 0, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p01_led_control, cpld_n_bits, CPLD1_QDD_P01_P02_LED_CONTROL, 0, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p02_led_control, cpld_n_bits, CPLD1_QDD_P01_P02_LED_CONTROL, 4, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p03_led_control, cpld_n_bits, CPLD1_QDD_P03_P04_LED_CONTROL, 0, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p04_led_control, cpld_n_bits, CPLD1_QDD_P03_P04_LED_CONTROL, 4, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p05_led_control, cpld_n_bits, CPLD1_QDD_P05_P06_LED_CONTROL, 0, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p06_led_control, cpld_n_bits, CPLD1_QDD_P05_P06_LED_CONTROL, 4, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p07_led_control, cpld_n_bits, CPLD1_QDD_P07_P08_LED_CONTROL, 0, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p08_led_control, cpld_n_bits, CPLD1_QDD_P07_P08_LED_CONTROL, 4, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p09_led_control, cpld_n_bits, CPLD1_QDD_P09_P10_LED_CONTROL, 0, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p10_led_control, cpld_n_bits, CPLD1_QDD_P09_P10_LED_CONTROL, 4, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p11_led_control, cpld_n_bits, CPLD1_QDD_P11_P12_LED_CONTROL, 0, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p12_led_control, cpld_n_bits, CPLD1_QDD_P11_P12_LED_CONTROL, 4, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p13_led_control, cpld_n_bits, CPLD1_QDD_P13_P14_LED_CONTROL, 0, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p14_led_control, cpld_n_bits, CPLD1_QDD_P13_P14_LED_CONTROL, 4, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p15_led_control, cpld_n_bits, CPLD1_QDD_P15_P16_LED_CONTROL, 0, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p16_led_control, cpld_n_bits, CPLD1_QDD_P15_P16_LED_CONTROL, 4, 3);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p01_present_interrupt_n, cpld_n_bits, CPLD1_QDD_P01_P08_INT_INSERT_STATUS, 0, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p02_present_interrupt_n, cpld_n_bits, CPLD1_QDD_P01_P08_INT_INSERT_STATUS, 1, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p03_present_interrupt_n, cpld_n_bits, CPLD1_QDD_P01_P08_INT_INSERT_STATUS, 2, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p04_present_interrupt_n, cpld_n_bits, CPLD1_QDD_P01_P08_INT_INSERT_STATUS, 3, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p05_present_interrupt_n, cpld_n_bits, CPLD1_QDD_P01_P08_INT_INSERT_STATUS, 4, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p06_present_interrupt_n, cpld_n_bits, CPLD1_QDD_P01_P08_INT_INSERT_STATUS, 5, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p07_present_interrupt_n, cpld_n_bits, CPLD1_QDD_P01_P08_INT_INSERT_STATUS, 6, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p08_present_interrupt_n, cpld_n_bits, CPLD1_QDD_P01_P08_INT_INSERT_STATUS, 7, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p01_p08_present_interrupt_n, cpld_n_bits, CPLD1_QDD_P01_P08_INT_INSERT_STATUS, 0, 8);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p09_present_interrupt_n, cpld_n_bits, CPLD1_QDD_P09_P16_INT_INSERT_STATUS, 0, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p10_present_interrupt_n, cpld_n_bits, CPLD1_QDD_P09_P16_INT_INSERT_STATUS, 1, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p11_present_interrupt_n, cpld_n_bits, CPLD1_QDD_P09_P16_INT_INSERT_STATUS, 2, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p12_present_interrupt_n, cpld_n_bits, CPLD1_QDD_P09_P16_INT_INSERT_STATUS, 3, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p13_present_interrupt_n, cpld_n_bits, CPLD1_QDD_P09_P16_INT_INSERT_STATUS, 4, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p14_present_interrupt_n, cpld_n_bits, CPLD1_QDD_P09_P16_INT_INSERT_STATUS, 5, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p15_present_interrupt_n, cpld_n_bits, CPLD1_QDD_P09_P16_INT_INSERT_STATUS, 6, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p16_present_interrupt_n, cpld_n_bits, CPLD1_QDD_P09_P16_INT_INSERT_STATUS, 7, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p09_p16_present_interrupt_n, cpld_n_bits, CPLD1_QDD_P09_P16_INT_INSERT_STATUS, 0, 8);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p01_p16_present_interrupt_n, cpld_n_bits, CPLD1_INTERRUPT_STATUS_1, 0, 1);

static struct attribute *system_cpld1_attrs[] = {
  &cpld_dev_attr_cpld1_minor_version.dev_attr.attr,
  &cpld_dev_attr_cpld1_major_version.dev_attr.attr,
  &cpld_dev_attr_cpld1_release_month.dev_attr.attr,
  &cpld_dev_attr_cpld1_release_year.dev_attr.attr,
  &cpld_dev_attr_cpld1_release_day.dev_attr.attr,
  &cpld_dev_attr_cpld1_build_rev.dev_attr.attr,
  &cpld_dev_attr_cpld1_hw_rev.dev_attr.attr,
  &cpld_dev_attr_cpld1_board_id.dev_attr.attr,
  &cpld_dev_attr_psu1_smbalert_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_psu1_present_n.dev_attr.attr,
  &cpld_dev_attr_psu1_pwrgd.dev_attr.attr,
  &cpld_dev_attr_psu1_acok.dev_attr.attr,
  &cpld_dev_attr_psu2_smbalert_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_psu2_present_n.dev_attr.attr,
  &cpld_dev_attr_psu2_pwrgd.dev_attr.attr,
  &cpld_dev_attr_psu2_acok.dev_attr.attr,
  &cpld_dev_attr_psu_on_n.dev_attr.attr,
  &cpld_dev_attr_psu1_pson_n.dev_attr.attr,
  &cpld_dev_attr_psu2_pson_n.dev_attr.attr,
  &cpld_dev_attr_p3v3_b_en.dev_attr.attr,
  &cpld_dev_attr_p3v3_a_en.dev_attr.attr,
  &cpld_dev_attr_bmc_pwrgd.dev_attr.attr,
  &cpld_dev_attr_psu_pwrgd.dev_attr.attr,
  &cpld_dev_attr_p3v3_b_pwrgd.dev_attr.attr,
  &cpld_dev_attr_p3v3_a_pwrgd.dev_attr.attr,
  &cpld_dev_attr_system_pwrgd.dev_attr.attr,
  &cpld_dev_attr_cpld1_rst_platrst_cpld1_n.dev_attr.attr,
  &cpld_dev_attr_cpld1_rst_swrst_cpld1_n.dev_attr.attr,
  &cpld_dev_attr_cpld1_cpld2_thermtrip_n.dev_attr.attr,
  &cpld_dev_attr_cpld1_thermtrip_n.dev_attr.attr,
  &cpld_dev_attr_en_cpld1_hitless_n.dev_attr.attr,
  &cpld_dev_attr_mb_eeprom_wc_n.dev_attr.attr,
  &cpld_dev_attr_rst_fancpld_n.dev_attr.attr,
  &cpld_dev_attr_rst_phy_n.dev_attr.attr,
  &cpld_dev_attr_rst_i2c_mux_d0_n.dev_attr.attr,
  &cpld_dev_attr_rst_i2c_mux_d1_n.dev_attr.attr,
  &cpld_dev_attr_rst_i2c_mux_d2_n.dev_attr.attr,
  &cpld_dev_attr_rst_i2c_mux_d3_n.dev_attr.attr,
  &cpld_dev_attr_rst_i2c_mux_d4_n.dev_attr.attr,
  &cpld_dev_attr_rst_i2c_mux_d5_n.dev_attr.attr,
  &cpld_dev_attr_rst_i2c_mux_d6_n.dev_attr.attr,
  &cpld_dev_attr_rst_i2c_mux_d7_n.dev_attr.attr,
  &cpld_dev_attr_rst_i2c_mux_d8_n.dev_attr.attr,
  &cpld_dev_attr_sys_led_control.dev_attr.attr,
  &cpld_dev_attr_psu_led_control.dev_attr.attr,
  &cpld_dev_attr_fan_led_control.dev_attr.attr,
  &cpld_dev_attr_fault_led_control.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p01_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p02_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p03_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p04_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p05_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p06_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p07_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p08_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p09_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p10_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p11_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p12_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p13_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p14_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p15_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p16_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p01_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p02_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p03_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p04_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p05_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p06_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p07_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p08_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p01_p08_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p09_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p10_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p11_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p12_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p13_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p14_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p15_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p16_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p09_p16_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p01_p16_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p01_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p02_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p03_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p04_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p05_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p06_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p07_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p08_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p01_p08_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p09_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p10_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p11_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p12_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p13_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p14_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p15_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p16_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p09_p16_present_n.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p01.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p02.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p03.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p04.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p05.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p06.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p07.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p08.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p09.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p10.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p11.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p12.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p13.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p14.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p15.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p16.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p01_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p02_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p03_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p04_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p05_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p06_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p07_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p08_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p09_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p10_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p11_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p12_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p13_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p14_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p15_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p16_n.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p01.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p02.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p03.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p04.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p05.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p06.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p07.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p08.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p09.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p10.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p11.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p12.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p13.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p14.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p15.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p16.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p01_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p02_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p03_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p04_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p05_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p06_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p07_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p08_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p09_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p10_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p11_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p12_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p13_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p14_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p15_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p16_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p01_p16_pwrgd_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p01_p16_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p01_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p02_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p03_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p04_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p05_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p06_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p07_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p08_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p09_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p10_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p11_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p12_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p13_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p14_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p15_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p16_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p01_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p02_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p03_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p04_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p05_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p06_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p07_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p08_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p01_p08_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p09_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p10_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p11_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p12_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p13_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p14_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p15_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p16_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p09_p16_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p01_p16_present_interrupt_n.dev_attr.attr,
  NULL,
};

enum system_cpld2_regs {
  CPLD2_BMC_INFO                      = 0x5,
  CPLD2_DC_POWER_GOOD                 = 0x6,
  CPLD2_SYSTEM_POWER_STATUS           = 0xe,
  CPLD2_RESET_STATUS                  = 0xf,
  CPLD2_THERMTRIP                     = 0x10,
  CPLD2_HITLESS                       = 0x11,
  CPLD2_RESET_QDD_P17_P24_CONTROL     = 0x22,
  CPLD2_RESET_QDD_P25_P32_CONTROL     = 0x23,
  CPLD2_INT_QDD_P17_P24_STATUS        = 0x24,
  CPLD2_INT_QDD_P25_P32_STATUS        = 0x25,
  CPLD2_MODPRSNT_QDD_P17_P24_STATUS   = 0x26,
  CPLD2_MODPRSNT_QDD_P25_P32_STATUS   = 0x27,
  CPLD2_LPMODE_QDD_P17_P24_CONTROL    = 0x28,
  CPLD2_LPMODE_QDD_P25_P32_CONTROL    = 0x29,
  CPLD2_MODSEL_QDD_P17_P24_CONTROL    = 0x2a,
  CPLD2_MODSEL_QDD_P25_P32_CONTROL    = 0x2b,
  CPLD2_EN_P3V3_QDD_P17_P24_CONTROL   = 0x2c,
  CPLD2_EN_P3V3_QDD_P25_P32_CONTROL   = 0x2e,
  CPLD2_PWRGD_P3V3_QDD_P17_P24_STATUS = 0x30,
  CPLD2_PWRGD_P3V3_QDD_P25_P32_STATUS = 0x31,
  CPLD2_QDD_P17_P32_ALL_LED_CONTROL   = 0x33,
  CPLD2_QDD_P17_P18_LED_CONTROL       = 0x34,
  CPLD2_QDD_P19_P20_LED_CONTROL       = 0x35,
  CPLD2_QDD_P21_P22_LED_CONTROL       = 0x36,
  CPLD2_QDD_P23_P24_LED_CONTROL       = 0x37,
  CPLD2_QDD_P25_P26_LED_CONTROL       = 0x38,
  CPLD2_QDD_P27_P28_LED_CONTROL       = 0x39,
  CPLD2_QDD_P29_P30_LED_CONTROL       = 0x3a,
  CPLD2_QDD_P31_P32_LED_CONTROL       = 0x3b,
  CPLD2_INTERRUPT_STATUS              = 0x3f,
  CPLD2_QDD_P17_P24_INT_INSERT_STATUS = 0x42,
  CPLD2_QDD_P25_P32_INT_INSERT_STATUS = 0x43,
};

static CPLD_DEVICE_ATTR_RO(cpld2_minor_version, cpld_n_bits, CPLD_VERSION, 0, 4);
static CPLD_DEVICE_ATTR_RO(cpld2_major_version, cpld_n_bits, CPLD_VERSION, 4, 4);
static CPLD_DEVICE_ATTR_RO(cpld2_release_month, cpld_n_bits, CPLD_RELEASE_DATE_1, 0, 4);
static CPLD_DEVICE_ATTR_RO(cpld2_release_year, cpld_n_bits, CPLD_RELEASE_DATE_1, 4, 4);
static CPLD_DEVICE_ATTR_RO(cpld2_release_day, cpld_n_bits, CPLD_RELEASE_DATE_2, 0, 4);
static CPLD_DEVICE_ATTR_RO(cpld2_rst_platrst_cpld1_n, cpld_n_bits, CPLD2_RESET_STATUS, 0, 1);
static CPLD_DEVICE_ATTR_RO(cpld2_rst_swrst_cpld1_n, cpld_n_bits, CPLD2_RESET_STATUS, 1, 1);
static CPLD_DEVICE_ATTR_RO(cpld2_cpld2_thermtrip_n, cpld_n_bits, CPLD2_THERMTRIP, 0, 1);
static CPLD_DEVICE_ATTR_RO(en_cpld2_hitless_n, cpld_n_bits, CPLD2_HITLESS, 0, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p17_n, cpld_n_bits, CPLD2_RESET_QDD_P17_P24_CONTROL, 0, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p18_n, cpld_n_bits, CPLD2_RESET_QDD_P17_P24_CONTROL, 1, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p19_n, cpld_n_bits, CPLD2_RESET_QDD_P17_P24_CONTROL, 2, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p20_n, cpld_n_bits, CPLD2_RESET_QDD_P17_P24_CONTROL, 3, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p21_n, cpld_n_bits, CPLD2_RESET_QDD_P17_P24_CONTROL, 4, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p22_n, cpld_n_bits, CPLD2_RESET_QDD_P17_P24_CONTROL, 5, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p23_n, cpld_n_bits, CPLD2_RESET_QDD_P17_P24_CONTROL, 6, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p24_n, cpld_n_bits, CPLD2_RESET_QDD_P17_P24_CONTROL, 7, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p25_n, cpld_n_bits, CPLD2_RESET_QDD_P25_P32_CONTROL, 0, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p26_n, cpld_n_bits, CPLD2_RESET_QDD_P25_P32_CONTROL, 1, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p27_n, cpld_n_bits, CPLD2_RESET_QDD_P25_P32_CONTROL, 2, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p28_n, cpld_n_bits, CPLD2_RESET_QDD_P25_P32_CONTROL, 3, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p29_n, cpld_n_bits, CPLD2_RESET_QDD_P25_P32_CONTROL, 4, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p30_n, cpld_n_bits, CPLD2_RESET_QDD_P25_P32_CONTROL, 5, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p31_n, cpld_n_bits, CPLD2_RESET_QDD_P25_P32_CONTROL, 6, 1);
static CPLD_DEVICE_ATTR_RW(rst_qsfpdd_p32_n, cpld_n_bits, CPLD2_RESET_QDD_P25_P32_CONTROL, 7, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p17_interrupt_n, cpld_n_bits, CPLD2_INT_QDD_P17_P24_STATUS, 0, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p18_interrupt_n, cpld_n_bits, CPLD2_INT_QDD_P17_P24_STATUS, 1, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p19_interrupt_n, cpld_n_bits, CPLD2_INT_QDD_P17_P24_STATUS, 2, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p20_interrupt_n, cpld_n_bits, CPLD2_INT_QDD_P17_P24_STATUS, 3, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p21_interrupt_n, cpld_n_bits, CPLD2_INT_QDD_P17_P24_STATUS, 4, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p22_interrupt_n, cpld_n_bits, CPLD2_INT_QDD_P17_P24_STATUS, 5, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p23_interrupt_n, cpld_n_bits, CPLD2_INT_QDD_P17_P24_STATUS, 6, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p24_interrupt_n, cpld_n_bits, CPLD2_INT_QDD_P17_P24_STATUS, 7, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p17_p24_interrupt_n, cpld_n_bits, CPLD2_INT_QDD_P17_P24_STATUS, 0, 8);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p25_interrupt_n, cpld_n_bits, CPLD2_INT_QDD_P25_P32_STATUS, 0, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p26_interrupt_n, cpld_n_bits, CPLD2_INT_QDD_P25_P32_STATUS, 1, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p27_interrupt_n, cpld_n_bits, CPLD2_INT_QDD_P25_P32_STATUS, 2, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p28_interrupt_n, cpld_n_bits, CPLD2_INT_QDD_P25_P32_STATUS, 3, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p29_interrupt_n, cpld_n_bits, CPLD2_INT_QDD_P25_P32_STATUS, 4, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p30_interrupt_n, cpld_n_bits, CPLD2_INT_QDD_P25_P32_STATUS, 5, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p31_interrupt_n, cpld_n_bits, CPLD2_INT_QDD_P25_P32_STATUS, 6, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p32_interrupt_n, cpld_n_bits, CPLD2_INT_QDD_P25_P32_STATUS, 7, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p25_p32_interrupt_n, cpld_n_bits, CPLD2_INT_QDD_P25_P32_STATUS, 0, 8);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p17_p32_interrupt_n, cpld_n_bits, CPLD2_INTERRUPT_STATUS, 1, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p17_present_n, cpld_n_bits, CPLD2_MODPRSNT_QDD_P17_P24_STATUS, 0, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p18_present_n, cpld_n_bits, CPLD2_MODPRSNT_QDD_P17_P24_STATUS, 1, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p19_present_n, cpld_n_bits, CPLD2_MODPRSNT_QDD_P17_P24_STATUS, 2, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p20_present_n, cpld_n_bits, CPLD2_MODPRSNT_QDD_P17_P24_STATUS, 3, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p21_present_n, cpld_n_bits, CPLD2_MODPRSNT_QDD_P17_P24_STATUS, 4, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p22_present_n, cpld_n_bits, CPLD2_MODPRSNT_QDD_P17_P24_STATUS, 5, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p23_present_n, cpld_n_bits, CPLD2_MODPRSNT_QDD_P17_P24_STATUS, 6, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p24_present_n, cpld_n_bits, CPLD2_MODPRSNT_QDD_P17_P24_STATUS, 7, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p17_p24_present_n, cpld_n_bits, CPLD2_MODPRSNT_QDD_P17_P24_STATUS, 0, 8);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p25_present_n, cpld_n_bits, CPLD2_MODPRSNT_QDD_P25_P32_STATUS, 0, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p26_present_n, cpld_n_bits, CPLD2_MODPRSNT_QDD_P25_P32_STATUS, 1, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p27_present_n, cpld_n_bits, CPLD2_MODPRSNT_QDD_P25_P32_STATUS, 2, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p28_present_n, cpld_n_bits, CPLD2_MODPRSNT_QDD_P25_P32_STATUS, 3, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p29_present_n, cpld_n_bits, CPLD2_MODPRSNT_QDD_P25_P32_STATUS, 4, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p30_present_n, cpld_n_bits, CPLD2_MODPRSNT_QDD_P25_P32_STATUS, 5, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p31_present_n, cpld_n_bits, CPLD2_MODPRSNT_QDD_P25_P32_STATUS, 6, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p32_present_n, cpld_n_bits, CPLD2_MODPRSNT_QDD_P25_P32_STATUS, 7, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p25_p32_present_n, cpld_n_bits, CPLD2_MODPRSNT_QDD_P25_P32_STATUS, 0, 8);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p17, cpld_n_bits, CPLD2_LPMODE_QDD_P17_P24_CONTROL, 0, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p18, cpld_n_bits, CPLD2_LPMODE_QDD_P17_P24_CONTROL, 1, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p19, cpld_n_bits, CPLD2_LPMODE_QDD_P17_P24_CONTROL, 2, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p20, cpld_n_bits, CPLD2_LPMODE_QDD_P17_P24_CONTROL, 3, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p21, cpld_n_bits, CPLD2_LPMODE_QDD_P17_P24_CONTROL, 4, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p22, cpld_n_bits, CPLD2_LPMODE_QDD_P17_P24_CONTROL, 5, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p23, cpld_n_bits, CPLD2_LPMODE_QDD_P17_P24_CONTROL, 6, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p24, cpld_n_bits, CPLD2_LPMODE_QDD_P17_P24_CONTROL, 7, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p25, cpld_n_bits, CPLD2_LPMODE_QDD_P25_P32_CONTROL, 0, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p26, cpld_n_bits, CPLD2_LPMODE_QDD_P25_P32_CONTROL, 1, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p27, cpld_n_bits, CPLD2_LPMODE_QDD_P25_P32_CONTROL, 2, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p28, cpld_n_bits, CPLD2_LPMODE_QDD_P25_P32_CONTROL, 3, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p29, cpld_n_bits, CPLD2_LPMODE_QDD_P25_P32_CONTROL, 4, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p30, cpld_n_bits, CPLD2_LPMODE_QDD_P25_P32_CONTROL, 5, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p31, cpld_n_bits, CPLD2_LPMODE_QDD_P25_P32_CONTROL, 6, 1);
static CPLD_DEVICE_ATTR_RW(lpmode_qsfpdd_p32, cpld_n_bits, CPLD2_LPMODE_QDD_P25_P32_CONTROL, 7, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p17_n, cpld_n_bits, CPLD2_MODSEL_QDD_P17_P24_CONTROL, 0, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p18_n, cpld_n_bits, CPLD2_MODSEL_QDD_P17_P24_CONTROL, 1, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p19_n, cpld_n_bits, CPLD2_MODSEL_QDD_P17_P24_CONTROL, 2, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p20_n, cpld_n_bits, CPLD2_MODSEL_QDD_P17_P24_CONTROL, 3, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p21_n, cpld_n_bits, CPLD2_MODSEL_QDD_P17_P24_CONTROL, 4, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p22_n, cpld_n_bits, CPLD2_MODSEL_QDD_P17_P24_CONTROL, 5, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p23_n, cpld_n_bits, CPLD2_MODSEL_QDD_P17_P24_CONTROL, 6, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p24_n, cpld_n_bits, CPLD2_MODSEL_QDD_P17_P24_CONTROL, 7, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p25_n, cpld_n_bits, CPLD2_MODSEL_QDD_P25_P32_CONTROL, 0, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p26_n, cpld_n_bits, CPLD2_MODSEL_QDD_P25_P32_CONTROL, 1, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p27_n, cpld_n_bits, CPLD2_MODSEL_QDD_P25_P32_CONTROL, 2, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p28_n, cpld_n_bits, CPLD2_MODSEL_QDD_P25_P32_CONTROL, 3, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p29_n, cpld_n_bits, CPLD2_MODSEL_QDD_P25_P32_CONTROL, 4, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p30_n, cpld_n_bits, CPLD2_MODSEL_QDD_P25_P32_CONTROL, 5, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p31_n, cpld_n_bits, CPLD2_MODSEL_QDD_P25_P32_CONTROL, 6, 1);
static CPLD_DEVICE_ATTR_RW(modsel_qsfpdd_p32_n, cpld_n_bits, CPLD2_MODSEL_QDD_P25_P32_CONTROL, 7, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p17, cpld_n_bits, CPLD2_EN_P3V3_QDD_P17_P24_CONTROL, 0, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p18, cpld_n_bits, CPLD2_EN_P3V3_QDD_P17_P24_CONTROL, 1, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p19, cpld_n_bits, CPLD2_EN_P3V3_QDD_P17_P24_CONTROL, 2, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p20, cpld_n_bits, CPLD2_EN_P3V3_QDD_P17_P24_CONTROL, 3, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p21, cpld_n_bits, CPLD2_EN_P3V3_QDD_P17_P24_CONTROL, 4, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p22, cpld_n_bits, CPLD2_EN_P3V3_QDD_P17_P24_CONTROL, 5, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p23, cpld_n_bits, CPLD2_EN_P3V3_QDD_P17_P24_CONTROL, 6, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p24, cpld_n_bits, CPLD2_EN_P3V3_QDD_P17_P24_CONTROL, 7, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p25, cpld_n_bits, CPLD2_EN_P3V3_QDD_P25_P32_CONTROL, 0, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p26, cpld_n_bits, CPLD2_EN_P3V3_QDD_P25_P32_CONTROL, 1, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p27, cpld_n_bits, CPLD2_EN_P3V3_QDD_P25_P32_CONTROL, 2, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p28, cpld_n_bits, CPLD2_EN_P3V3_QDD_P25_P32_CONTROL, 3, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p29, cpld_n_bits, CPLD2_EN_P3V3_QDD_P25_P32_CONTROL, 4, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p30, cpld_n_bits, CPLD2_EN_P3V3_QDD_P25_P32_CONTROL, 5, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p31, cpld_n_bits, CPLD2_EN_P3V3_QDD_P25_P32_CONTROL, 6, 1);
static CPLD_DEVICE_ATTR_RW(en_p3v3_qsfpdd_p32, cpld_n_bits, CPLD2_EN_P3V3_QDD_P25_P32_CONTROL, 7, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p17_pwrgd, cpld_n_bits, CPLD2_PWRGD_P3V3_QDD_P17_P24_STATUS, 0, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p18_pwrgd, cpld_n_bits, CPLD2_PWRGD_P3V3_QDD_P17_P24_STATUS, 1, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p19_pwrgd, cpld_n_bits, CPLD2_PWRGD_P3V3_QDD_P17_P24_STATUS, 2, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p20_pwrgd, cpld_n_bits, CPLD2_PWRGD_P3V3_QDD_P17_P24_STATUS, 3, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p21_pwrgd, cpld_n_bits, CPLD2_PWRGD_P3V3_QDD_P17_P24_STATUS, 4, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p22_pwrgd, cpld_n_bits, CPLD2_PWRGD_P3V3_QDD_P17_P24_STATUS, 5, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p23_pwrgd, cpld_n_bits, CPLD2_PWRGD_P3V3_QDD_P17_P24_STATUS, 6, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p24_pwrgd, cpld_n_bits, CPLD2_PWRGD_P3V3_QDD_P17_P24_STATUS, 7, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p25_pwrgd, cpld_n_bits, CPLD2_PWRGD_P3V3_QDD_P25_P32_STATUS, 0, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p26_pwrgd, cpld_n_bits, CPLD2_PWRGD_P3V3_QDD_P25_P32_STATUS, 1, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p27_pwrgd, cpld_n_bits, CPLD2_PWRGD_P3V3_QDD_P25_P32_STATUS, 2, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p28_pwrgd, cpld_n_bits, CPLD2_PWRGD_P3V3_QDD_P25_P32_STATUS, 3, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p29_pwrgd, cpld_n_bits, CPLD2_PWRGD_P3V3_QDD_P25_P32_STATUS, 4, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p30_pwrgd, cpld_n_bits, CPLD2_PWRGD_P3V3_QDD_P25_P32_STATUS, 5, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p31_pwrgd, cpld_n_bits, CPLD2_PWRGD_P3V3_QDD_P25_P32_STATUS, 6, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p32_pwrgd, cpld_n_bits, CPLD2_PWRGD_P3V3_QDD_P25_P32_STATUS, 7, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p17_p32_pwrgd_interrupt_n, cpld_n_bits, CPLD2_INTERRUPT_STATUS, 2, 1);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p17_p32_led_control, cpld_n_bits, CPLD2_QDD_P17_P32_ALL_LED_CONTROL, 0, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p17_led_control, cpld_n_bits, CPLD2_QDD_P17_P18_LED_CONTROL, 0, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p18_led_control, cpld_n_bits, CPLD2_QDD_P17_P18_LED_CONTROL, 4, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p19_led_control, cpld_n_bits, CPLD2_QDD_P19_P20_LED_CONTROL, 0, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p20_led_control, cpld_n_bits, CPLD2_QDD_P19_P20_LED_CONTROL, 4, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p21_led_control, cpld_n_bits, CPLD2_QDD_P21_P22_LED_CONTROL, 0, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p22_led_control, cpld_n_bits, CPLD2_QDD_P21_P22_LED_CONTROL, 4, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p23_led_control, cpld_n_bits, CPLD2_QDD_P23_P24_LED_CONTROL, 0, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p24_led_control, cpld_n_bits, CPLD2_QDD_P23_P24_LED_CONTROL, 4, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p25_led_control, cpld_n_bits, CPLD2_QDD_P25_P26_LED_CONTROL, 0, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p26_led_control, cpld_n_bits, CPLD2_QDD_P25_P26_LED_CONTROL, 4, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p27_led_control, cpld_n_bits, CPLD2_QDD_P27_P28_LED_CONTROL, 0, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p28_led_control, cpld_n_bits, CPLD2_QDD_P27_P28_LED_CONTROL, 4, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p29_led_control, cpld_n_bits, CPLD2_QDD_P29_P30_LED_CONTROL, 0, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p30_led_control, cpld_n_bits, CPLD2_QDD_P29_P30_LED_CONTROL, 4, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p31_led_control, cpld_n_bits, CPLD2_QDD_P31_P32_LED_CONTROL, 0, 3);
static CPLD_DEVICE_ATTR_RW(qsfpdd_p32_led_control, cpld_n_bits, CPLD2_QDD_P31_P32_LED_CONTROL, 4, 3);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p17_present_interrupt_n, cpld_n_bits, CPLD2_QDD_P17_P24_INT_INSERT_STATUS, 0, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p18_present_interrupt_n, cpld_n_bits, CPLD2_QDD_P17_P24_INT_INSERT_STATUS, 1, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p19_present_interrupt_n, cpld_n_bits, CPLD2_QDD_P17_P24_INT_INSERT_STATUS, 2, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p20_present_interrupt_n, cpld_n_bits, CPLD2_QDD_P17_P24_INT_INSERT_STATUS, 3, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p21_present_interrupt_n, cpld_n_bits, CPLD2_QDD_P17_P24_INT_INSERT_STATUS, 4, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p22_present_interrupt_n, cpld_n_bits, CPLD2_QDD_P17_P24_INT_INSERT_STATUS, 5, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p23_present_interrupt_n, cpld_n_bits, CPLD2_QDD_P17_P24_INT_INSERT_STATUS, 6, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p24_present_interrupt_n, cpld_n_bits, CPLD2_QDD_P17_P24_INT_INSERT_STATUS, 7, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p17_p24_present_interrupt_n, cpld_n_bits, CPLD2_QDD_P17_P24_INT_INSERT_STATUS, 0, 8);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p25_present_interrupt_n, cpld_n_bits, CPLD2_QDD_P25_P32_INT_INSERT_STATUS, 0, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p26_present_interrupt_n, cpld_n_bits, CPLD2_QDD_P25_P32_INT_INSERT_STATUS, 1, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p27_present_interrupt_n, cpld_n_bits, CPLD2_QDD_P25_P32_INT_INSERT_STATUS, 2, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p28_present_interrupt_n, cpld_n_bits, CPLD2_QDD_P25_P32_INT_INSERT_STATUS, 3, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p29_present_interrupt_n, cpld_n_bits, CPLD2_QDD_P25_P32_INT_INSERT_STATUS, 4, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p30_present_interrupt_n, cpld_n_bits, CPLD2_QDD_P25_P32_INT_INSERT_STATUS, 5, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p31_present_interrupt_n, cpld_n_bits, CPLD2_QDD_P25_P32_INT_INSERT_STATUS, 6, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p32_present_interrupt_n, cpld_n_bits, CPLD2_QDD_P25_P32_INT_INSERT_STATUS, 7, 1);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p25_p32_present_interrupt_n, cpld_n_bits, CPLD2_QDD_P25_P32_INT_INSERT_STATUS, 0, 8);
static CPLD_DEVICE_ATTR_RO(qsfpdd_p17_p32_present_interrupt_n, cpld_n_bits, CPLD2_INTERRUPT_STATUS, 0, 1);

static struct attribute *system_cpld2_attrs[] = {
  &cpld_dev_attr_cpld2_minor_version.dev_attr.attr,
  &cpld_dev_attr_cpld2_major_version.dev_attr.attr,
  &cpld_dev_attr_cpld2_release_month.dev_attr.attr,
  &cpld_dev_attr_cpld2_release_year.dev_attr.attr,
  &cpld_dev_attr_cpld2_release_day.dev_attr.attr,
  &cpld_dev_attr_cpld2_rst_swrst_cpld1_n.dev_attr.attr,
  &cpld_dev_attr_cpld2_rst_platrst_cpld1_n.dev_attr.attr,
  &cpld_dev_attr_cpld2_cpld2_thermtrip_n.dev_attr.attr,
  &cpld_dev_attr_en_cpld2_hitless_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p17_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p18_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p19_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p20_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p21_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p22_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p23_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p24_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p25_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p26_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p27_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p28_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p29_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p30_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p31_n.dev_attr.attr,
  &cpld_dev_attr_rst_qsfpdd_p32_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p17_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p18_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p19_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p20_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p21_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p22_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p23_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p24_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p17_p24_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p25_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p26_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p27_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p28_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p29_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p30_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p31_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p32_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p25_p32_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p17_p32_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p17_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p18_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p19_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p20_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p21_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p22_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p23_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p24_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p17_p24_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p25_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p26_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p27_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p28_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p29_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p30_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p31_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p32_present_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p25_p32_present_n.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p17.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p18.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p19.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p20.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p21.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p22.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p23.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p24.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p25.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p26.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p27.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p28.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p29.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p30.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p31.dev_attr.attr,
  &cpld_dev_attr_lpmode_qsfpdd_p32.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p17_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p18_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p19_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p20_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p21_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p22_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p23_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p24_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p25_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p26_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p27_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p28_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p29_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p30_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p31_n.dev_attr.attr,
  &cpld_dev_attr_modsel_qsfpdd_p32_n.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p17.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p18.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p19.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p20.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p21.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p22.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p23.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p24.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p25.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p26.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p27.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p28.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p29.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p30.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p31.dev_attr.attr,
  &cpld_dev_attr_en_p3v3_qsfpdd_p32.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p17_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p18_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p19_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p20_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p21_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p22_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p23_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p24_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p25_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p26_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p27_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p28_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p29_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p30_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p31_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p32_pwrgd.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p17_p32_pwrgd_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p17_p32_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p17_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p18_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p19_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p20_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p21_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p22_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p23_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p24_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p25_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p26_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p27_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p28_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p29_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p30_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p31_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p32_led_control.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p17_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p18_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p19_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p20_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p21_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p22_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p23_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p24_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p17_p24_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p25_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p26_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p27_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p28_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p29_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p30_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p31_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p32_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p25_p32_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_qsfpdd_p17_p32_present_interrupt_n.dev_attr.attr,
  NULL,
};

enum fancpld_regs {
  FANCPLD_BMC_STATUS                  = 0x5,
  FANCPLD_FM_PWM_FAN1_CONTROL         = 0x10,
  FANCPLD_FM_PWM_FAN2_CONTROL         = 0x11,
  FANCPLD_FM_PWM_FAN3_CONTROL         = 0x12,
  FANCPLD_FM_PWM_FAN4_CONTROL         = 0x13,
  FANCPLD_FM_PWM_FAN5_CONTROL         = 0x14,
  FANCPLD_FM_PWM_FAN6_CONTROL         = 0x15,
  FANCPLD_FM_TACH_F_FAN1_CALCULATION  = 0x16,
  FANCPLD_FM_TACH_R_FAN1_CALCULATION  = 0x17,
  FANCPLD_FM_TACH_F_FAN2_CALCULATION  = 0x18,
  FANCPLD_FM_TACH_R_FAN2_CALCULATION  = 0x19,
  FANCPLD_FM_TACH_F_FAN3_CALCULATION  = 0x1a,
  FANCPLD_FM_TACH_R_FAN3_CALCULATION  = 0x1b,
  FANCPLD_FM_TACH_F_FAN4_CALCULATION  = 0x1c,
  FANCPLD_FM_TACH_R_FAN4_CALCULATION  = 0x1d,
  FANCPLD_FM_TACH_F_FAN5_CALCULATION  = 0x1e,
  FANCPLD_FM_TACH_R_FAN5_CALCULATION  = 0x1f,
  FANCPLD_FM_TACH_F_FAN6_CALCULATION  = 0x20,
  FANCPLD_FM_TACH_R_FAN6_CALCULATION  = 0x21,
  FANCPLD_FM_PRSNT_FAN1_6_N_STATUS    = 0x30,
  FANCPLD_FM_DIR_FAN1_6_N_STATUS      = 0x31,
  FANCPLD_REAR_FAN_LED1_CONTROL       = 0x32,
  FANCPLD_REAR_FAN_LED2_CONTROL       = 0x33,
  FANCPLD_REAR_FAN_LED3_CONTROL       = 0x34,
  FANCPLD_RESET_CONTROL               = 0x35,
  FANCPLD_EN_FAN1_6_CONTROL           = 0x36,
  FANCPLD_PWRGD_P12V_FAN1_6_STATUS    = 0x37,
  FANCPLD_FM_FAN1_6_EEPROM_WC_CONTROL = 0x38,
  FANCPLD_FAN_INT_INSERTION           = 0x39,
  FANCPLD_FAN_FAIL                    = 0x3a,
  FANCPLD_INTERRUPT                   = 0x3b,
};

static CPLD_DEVICE_ATTR_RO(fancpld_minor_version, cpld_n_bits, CPLD_VERSION, 0, 4);
static CPLD_DEVICE_ATTR_RO(fancpld_major_version, cpld_n_bits, CPLD_VERSION, 4, 4);
static CPLD_DEVICE_ATTR_RO(fancpld_release_month, cpld_n_bits, CPLD_RELEASE_DATE_1, 0, 4);
static CPLD_DEVICE_ATTR_RO(fancpld_release_year, cpld_n_bits, CPLD_RELEASE_DATE_1, 4, 4);
static CPLD_DEVICE_ATTR_RO(fancpld_release_day, cpld_n_bits, CPLD_RELEASE_DATE_2, 0, 4);
static CPLD_DEVICE_ATTR_RO(fancpld_build_rev, cpld_n_bits, CPLD_BOARD_ID, 0, 2);
static CPLD_DEVICE_ATTR_RO(fancpld_hw_rev, cpld_n_bits, CPLD_BOARD_ID, 2, 2);
static CPLD_DEVICE_ATTR_RO(fancpld_board_id, cpld_n_bits, CPLD_BOARD_ID, 4, 4);
static CPLD_DEVICE_ATTR_RW(pwm1, fan_pwm, FANCPLD_FM_PWM_FAN1_CONTROL, 0, 8);
static CPLD_DEVICE_ATTR_RW(pwm2, fan_pwm, FANCPLD_FM_PWM_FAN2_CONTROL, 0, 8);
static CPLD_DEVICE_ATTR_RW(pwm3, fan_pwm, FANCPLD_FM_PWM_FAN3_CONTROL, 0, 8);
static CPLD_DEVICE_ATTR_RW(pwm4, fan_pwm, FANCPLD_FM_PWM_FAN4_CONTROL, 0, 8);
static CPLD_DEVICE_ATTR_RW(pwm5, fan_pwm, FANCPLD_FM_PWM_FAN5_CONTROL, 0, 8);
static CPLD_DEVICE_ATTR_RW(pwm6, fan_pwm, FANCPLD_FM_PWM_FAN6_CONTROL, 0, 8);
static CPLD_DEVICE_ATTR_RO(fan1_input, fan_rpm, FANCPLD_FM_TACH_F_FAN1_CALCULATION, 0, 8);
static CPLD_DEVICE_ATTR_RO(fan2_input, fan_rpm, FANCPLD_FM_TACH_R_FAN1_CALCULATION, 0, 8);
static CPLD_DEVICE_ATTR_RO(fan3_input, fan_rpm, FANCPLD_FM_TACH_F_FAN2_CALCULATION, 0, 8);
static CPLD_DEVICE_ATTR_RO(fan4_input, fan_rpm, FANCPLD_FM_TACH_R_FAN2_CALCULATION, 0, 8);
static CPLD_DEVICE_ATTR_RO(fan5_input, fan_rpm, FANCPLD_FM_TACH_F_FAN3_CALCULATION, 0, 8);
static CPLD_DEVICE_ATTR_RO(fan6_input, fan_rpm, FANCPLD_FM_TACH_R_FAN3_CALCULATION, 0, 8);
static CPLD_DEVICE_ATTR_RO(fan7_input, fan_rpm, FANCPLD_FM_TACH_F_FAN4_CALCULATION, 0, 8);
static CPLD_DEVICE_ATTR_RO(fan8_input, fan_rpm, FANCPLD_FM_TACH_R_FAN4_CALCULATION, 0, 8);
static CPLD_DEVICE_ATTR_RO(fan9_input, fan_rpm, FANCPLD_FM_TACH_F_FAN5_CALCULATION, 0, 8);
static CPLD_DEVICE_ATTR_RO(fan10_input, fan_rpm, FANCPLD_FM_TACH_R_FAN5_CALCULATION, 0, 8);
static CPLD_DEVICE_ATTR_RO(fan11_input, fan_rpm, FANCPLD_FM_TACH_F_FAN6_CALCULATION, 0, 8);
static CPLD_DEVICE_ATTR_RO(fan12_input, fan_rpm, FANCPLD_FM_TACH_R_FAN6_CALCULATION, 0, 8);
static CPLD_DEVICE_ATTR_RW(fan1_enable, cpld_n_bits, FANCPLD_EN_FAN1_6_CONTROL, 0, 1);
static CPLD_DEVICE_ATTR_RW(fan2_enable, cpld_n_bits, FANCPLD_EN_FAN1_6_CONTROL, 1, 1);
static CPLD_DEVICE_ATTR_RW(fan3_enable, cpld_n_bits, FANCPLD_EN_FAN1_6_CONTROL, 2, 1);
static CPLD_DEVICE_ATTR_RW(fan4_enable, cpld_n_bits, FANCPLD_EN_FAN1_6_CONTROL, 3, 1);
static CPLD_DEVICE_ATTR_RW(fan5_enable, cpld_n_bits, FANCPLD_EN_FAN1_6_CONTROL, 4, 1);
static CPLD_DEVICE_ATTR_RW(fan6_enable, cpld_n_bits, FANCPLD_EN_FAN1_6_CONTROL, 5, 1);
static CPLD_DEVICE_ATTR_RO(fan1_direction, cpld_n_bits, FANCPLD_FM_DIR_FAN1_6_N_STATUS, 0, 1);
static CPLD_DEVICE_ATTR_RO(fan2_direction, cpld_n_bits, FANCPLD_FM_DIR_FAN1_6_N_STATUS, 1, 1);
static CPLD_DEVICE_ATTR_RO(fan3_direction, cpld_n_bits, FANCPLD_FM_DIR_FAN1_6_N_STATUS, 2, 1);
static CPLD_DEVICE_ATTR_RO(fan4_direction, cpld_n_bits, FANCPLD_FM_DIR_FAN1_6_N_STATUS, 3, 1);
static CPLD_DEVICE_ATTR_RO(fan5_direction, cpld_n_bits, FANCPLD_FM_DIR_FAN1_6_N_STATUS, 4, 1);
static CPLD_DEVICE_ATTR_RO(fan6_direction, cpld_n_bits, FANCPLD_FM_DIR_FAN1_6_N_STATUS, 5, 1);
static CPLD_DEVICE_ATTR_RO(fan1_present_n, cpld_n_bits, FANCPLD_FM_PRSNT_FAN1_6_N_STATUS, 0, 1);
static CPLD_DEVICE_ATTR_RO(fan2_present_n, cpld_n_bits, FANCPLD_FM_PRSNT_FAN1_6_N_STATUS, 1, 1);
static CPLD_DEVICE_ATTR_RO(fan3_present_n, cpld_n_bits, FANCPLD_FM_PRSNT_FAN1_6_N_STATUS, 2, 1);
static CPLD_DEVICE_ATTR_RO(fan4_present_n, cpld_n_bits, FANCPLD_FM_PRSNT_FAN1_6_N_STATUS, 3, 1);
static CPLD_DEVICE_ATTR_RO(fan5_present_n, cpld_n_bits, FANCPLD_FM_PRSNT_FAN1_6_N_STATUS, 4, 1);
static CPLD_DEVICE_ATTR_RO(fan6_present_n, cpld_n_bits, FANCPLD_FM_PRSNT_FAN1_6_N_STATUS, 5, 1);
static CPLD_DEVICE_ATTR_RO(fan_present_n, cpld_n_bits, FANCPLD_FM_PRSNT_FAN1_6_N_STATUS, 0, 6);
static CPLD_DEVICE_ATTR_RO(fan1_present_interrupt_n, cpld_n_bits, FANCPLD_FAN_INT_INSERTION, 0, 1);
static CPLD_DEVICE_ATTR_RO(fan2_present_interrupt_n, cpld_n_bits, FANCPLD_FAN_INT_INSERTION, 1, 1);
static CPLD_DEVICE_ATTR_RO(fan3_present_interrupt_n, cpld_n_bits, FANCPLD_FAN_INT_INSERTION, 2, 1);
static CPLD_DEVICE_ATTR_RO(fan4_present_interrupt_n, cpld_n_bits, FANCPLD_FAN_INT_INSERTION, 3, 1);
static CPLD_DEVICE_ATTR_RO(fan5_present_interrupt_n, cpld_n_bits, FANCPLD_FAN_INT_INSERTION, 4, 1);
static CPLD_DEVICE_ATTR_RO(fan6_present_interrupt_n, cpld_n_bits, FANCPLD_FAN_INT_INSERTION, 5, 1);
static CPLD_DEVICE_ATTR_RO(fan_all_present_interrupt_n, cpld_n_bits, FANCPLD_FAN_INT_INSERTION, 0, 6);
static CPLD_DEVICE_ATTR_RW(rst_fan_n, cpld_n_bits, FANCPLD_RESET_CONTROL, 0, 1);
static CPLD_DEVICE_ATTR_RW(rst_i2c_mux_n, cpld_n_bits, FANCPLD_RESET_CONTROL, 1, 1);
static CPLD_DEVICE_ATTR_RW(fan1_led, cpld_n_bits, FANCPLD_REAR_FAN_LED1_CONTROL, 0, 3);
static CPLD_DEVICE_ATTR_RW(fan2_led, cpld_n_bits, FANCPLD_REAR_FAN_LED1_CONTROL, 4, 3);
static CPLD_DEVICE_ATTR_RW(fan3_led, cpld_n_bits, FANCPLD_REAR_FAN_LED2_CONTROL, 0, 3);
static CPLD_DEVICE_ATTR_RW(fan4_led, cpld_n_bits, FANCPLD_REAR_FAN_LED2_CONTROL, 4, 3);
static CPLD_DEVICE_ATTR_RW(fan5_led, cpld_n_bits, FANCPLD_REAR_FAN_LED3_CONTROL, 0, 3);
static CPLD_DEVICE_ATTR_RW(fan6_led, cpld_n_bits, FANCPLD_REAR_FAN_LED3_CONTROL, 4, 3);
static CPLD_DEVICE_ATTR_RO(fan1_eeprom_wc, cpld_n_bits, FANCPLD_FM_FAN1_6_EEPROM_WC_CONTROL, 0, 1);
static CPLD_DEVICE_ATTR_RO(fan2_eeprom_wc, cpld_n_bits, FANCPLD_FM_FAN1_6_EEPROM_WC_CONTROL, 1, 1);
static CPLD_DEVICE_ATTR_RO(fan3_eeprom_wc, cpld_n_bits, FANCPLD_FM_FAN1_6_EEPROM_WC_CONTROL, 2, 1);
static CPLD_DEVICE_ATTR_RO(fan4_eeprom_wc, cpld_n_bits, FANCPLD_FM_FAN1_6_EEPROM_WC_CONTROL, 3, 1);
static CPLD_DEVICE_ATTR_RO(fan5_eeprom_wc, cpld_n_bits, FANCPLD_FM_FAN1_6_EEPROM_WC_CONTROL, 4, 1);
static CPLD_DEVICE_ATTR_RO(fan6_eeprom_wc, cpld_n_bits, FANCPLD_FM_FAN1_6_EEPROM_WC_CONTROL, 5, 1);
static CPLD_DEVICE_ATTR_RO(fan1_pwrgd, cpld_n_bits, FANCPLD_PWRGD_P12V_FAN1_6_STATUS, 0, 1);
static CPLD_DEVICE_ATTR_RO(fan2_pwrgd, cpld_n_bits, FANCPLD_PWRGD_P12V_FAN1_6_STATUS, 1, 1);
static CPLD_DEVICE_ATTR_RO(fan3_pwrgd, cpld_n_bits, FANCPLD_PWRGD_P12V_FAN1_6_STATUS, 2, 1);
static CPLD_DEVICE_ATTR_RO(fan4_pwrgd, cpld_n_bits, FANCPLD_PWRGD_P12V_FAN1_6_STATUS, 3, 1);
static CPLD_DEVICE_ATTR_RO(fan5_pwrgd, cpld_n_bits, FANCPLD_PWRGD_P12V_FAN1_6_STATUS, 4, 1);
static CPLD_DEVICE_ATTR_RO(fan6_pwrgd, cpld_n_bits, FANCPLD_PWRGD_P12V_FAN1_6_STATUS, 5, 1);
static CPLD_DEVICE_ATTR_RO(fan_all_pwrgd, cpld_n_bits, FANCPLD_PWRGD_P12V_FAN1_6_STATUS, 0, 6);
static CPLD_DEVICE_ATTR_RO(fan1_fail_interrupt_n, cpld_n_bits, FANCPLD_FAN_FAIL, 0, 1);
static CPLD_DEVICE_ATTR_RO(fan2_fail_interrupt_n, cpld_n_bits, FANCPLD_FAN_FAIL, 1, 1);
static CPLD_DEVICE_ATTR_RO(fan3_fail_interrupt_n, cpld_n_bits, FANCPLD_FAN_FAIL, 2, 1);
static CPLD_DEVICE_ATTR_RO(fan4_fail_interrupt_n, cpld_n_bits, FANCPLD_FAN_FAIL, 3, 1);
static CPLD_DEVICE_ATTR_RO(fan5_fail_interrupt_n, cpld_n_bits, FANCPLD_FAN_FAIL, 4, 1);
static CPLD_DEVICE_ATTR_RO(fan6_fail_interrupt_n, cpld_n_bits, FANCPLD_FAN_FAIL, 5, 1);
static CPLD_DEVICE_ATTR_RO(fan_all_fail_interrupt_n, cpld_n_bits, FANCPLD_FAN_FAIL, 0, 6);
static CPLD_DEVICE_ATTR_RO(fan_fail_interrupt_n, cpld_n_bits, FANCPLD_INTERRUPT, 4, 1);

static struct attribute *fan_cpld_attrs[] = {
  &cpld_dev_attr_fancpld_minor_version.dev_attr.attr,
  &cpld_dev_attr_fancpld_major_version.dev_attr.attr,
  &cpld_dev_attr_fancpld_release_month.dev_attr.attr,
  &cpld_dev_attr_fancpld_release_year.dev_attr.attr,
  &cpld_dev_attr_fancpld_release_day.dev_attr.attr,
  &cpld_dev_attr_fancpld_build_rev.dev_attr.attr,
  &cpld_dev_attr_fancpld_hw_rev.dev_attr.attr,
  &cpld_dev_attr_fancpld_board_id.dev_attr.attr,
  &cpld_dev_attr_pwm1.dev_attr.attr,
  &cpld_dev_attr_pwm2.dev_attr.attr,
  &cpld_dev_attr_pwm3.dev_attr.attr,
  &cpld_dev_attr_pwm4.dev_attr.attr,
  &cpld_dev_attr_pwm5.dev_attr.attr,
  &cpld_dev_attr_pwm6.dev_attr.attr,
  &cpld_dev_attr_fan1_input.dev_attr.attr,
  &cpld_dev_attr_fan2_input.dev_attr.attr,
  &cpld_dev_attr_fan3_input.dev_attr.attr,
  &cpld_dev_attr_fan4_input.dev_attr.attr,
  &cpld_dev_attr_fan5_input.dev_attr.attr,
  &cpld_dev_attr_fan6_input.dev_attr.attr,
  &cpld_dev_attr_fan7_input.dev_attr.attr,
  &cpld_dev_attr_fan8_input.dev_attr.attr,
  &cpld_dev_attr_fan9_input.dev_attr.attr,
  &cpld_dev_attr_fan10_input.dev_attr.attr,
  &cpld_dev_attr_fan11_input.dev_attr.attr,
  &cpld_dev_attr_fan12_input.dev_attr.attr,
  &cpld_dev_attr_fan1_enable.dev_attr.attr,
  &cpld_dev_attr_fan2_enable.dev_attr.attr,
  &cpld_dev_attr_fan3_enable.dev_attr.attr,
  &cpld_dev_attr_fan4_enable.dev_attr.attr,
  &cpld_dev_attr_fan5_enable.dev_attr.attr,
  &cpld_dev_attr_fan6_enable.dev_attr.attr,
  &cpld_dev_attr_fan1_direction.dev_attr.attr,
  &cpld_dev_attr_fan2_direction.dev_attr.attr,
  &cpld_dev_attr_fan3_direction.dev_attr.attr,
  &cpld_dev_attr_fan4_direction.dev_attr.attr,
  &cpld_dev_attr_fan5_direction.dev_attr.attr,
  &cpld_dev_attr_fan6_direction.dev_attr.attr,
  &cpld_dev_attr_fan1_present_n.dev_attr.attr,
  &cpld_dev_attr_fan2_present_n.dev_attr.attr,
  &cpld_dev_attr_fan3_present_n.dev_attr.attr,
  &cpld_dev_attr_fan4_present_n.dev_attr.attr,
  &cpld_dev_attr_fan5_present_n.dev_attr.attr,
  &cpld_dev_attr_fan6_present_n.dev_attr.attr,
  &cpld_dev_attr_fan_present_n.dev_attr.attr,
  &cpld_dev_attr_fan1_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_fan2_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_fan3_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_fan4_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_fan5_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_fan6_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_fan_all_present_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_rst_i2c_mux_n.dev_attr.attr,
  &cpld_dev_attr_rst_fan_n.dev_attr.attr,
  &cpld_dev_attr_fan1_led.dev_attr.attr,
  &cpld_dev_attr_fan2_led.dev_attr.attr,
  &cpld_dev_attr_fan3_led.dev_attr.attr,
  &cpld_dev_attr_fan4_led.dev_attr.attr,
  &cpld_dev_attr_fan5_led.dev_attr.attr,
  &cpld_dev_attr_fan6_led.dev_attr.attr,
  &cpld_dev_attr_fan1_pwrgd.dev_attr.attr,
  &cpld_dev_attr_fan2_pwrgd.dev_attr.attr,
  &cpld_dev_attr_fan3_pwrgd.dev_attr.attr,
  &cpld_dev_attr_fan4_pwrgd.dev_attr.attr,
  &cpld_dev_attr_fan5_pwrgd.dev_attr.attr,
  &cpld_dev_attr_fan6_pwrgd.dev_attr.attr,
  &cpld_dev_attr_fan1_eeprom_wc.dev_attr.attr,
  &cpld_dev_attr_fan2_eeprom_wc.dev_attr.attr,
  &cpld_dev_attr_fan3_eeprom_wc.dev_attr.attr,
  &cpld_dev_attr_fan4_eeprom_wc.dev_attr.attr,
  &cpld_dev_attr_fan5_eeprom_wc.dev_attr.attr,
  &cpld_dev_attr_fan6_eeprom_wc.dev_attr.attr,
  &cpld_dev_attr_fan_all_pwrgd.dev_attr.attr,
  &cpld_dev_attr_fan1_fail_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_fan2_fail_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_fan3_fail_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_fan4_fail_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_fan5_fail_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_fan6_fail_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_fan_all_fail_interrupt_n.dev_attr.attr,
  &cpld_dev_attr_fan_fail_interrupt_n.dev_attr.attr,
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
#define MAX_FAN_CPLD_GPIO 6
  [0] = {
    .reg = FANCPLD_FM_PRSNT_FAN1_6_N_STATUS,
    .shift = 0,
    .direction = 1
  },
  [1] = {
    .reg = FANCPLD_FM_PRSNT_FAN1_6_N_STATUS,
    .shift = 1,
    .direction = 1
  },
  [2] = {
    .reg = FANCPLD_FM_PRSNT_FAN1_6_N_STATUS,
    .shift = 2,
    .direction = 1
  },
  [3] = {
    .reg = FANCPLD_FM_PRSNT_FAN1_6_N_STATUS,
    .shift = 3,
    .direction = 1
  },
  [4] = {
    .reg = FANCPLD_FM_PRSNT_FAN1_6_N_STATUS,
    .shift = 4,
    .direction = 1
  },
  [5] = {
    .reg = FANCPLD_FM_PRSNT_FAN1_6_N_STATUS,
    .shift = 5,
    .direction = 1
  },
  NULL,
};

static struct cpld_gpio_info fpga_gpios[] = {
/* Define gpio numbers for FPGA */
#define MAX_FPGA_GPIO 0
  NULL,
};

#endif
