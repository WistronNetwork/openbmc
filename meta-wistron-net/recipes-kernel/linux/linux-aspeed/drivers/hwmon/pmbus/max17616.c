// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Hardware monitoring driver for Maxim MAX17616
 *
 * Copyright (c) 2023 Jarvis Chen. All rights reserved.
 */

#include <linux/bitops.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/i2c.h>
#include "pmbus.h"

enum chips { max17616 };

struct max17616_data {
	int id;
	struct pmbus_driver_info info;
};

#define to_max17616_data(x)  container_of(x, struct max17616_data, info)

static const struct i2c_device_id max17616_id[];

static struct pmbus_driver_info max17616_info[] = {
	[max17616] = {
		.pages = 1,
		.format[PSC_VOLTAGE_IN] = direct,
		.format[PSC_VOLTAGE_OUT] = direct,
		.format[PSC_TEMPERATURE] = direct,
		.format[PSC_CURRENT_OUT] = direct,
		.m[PSC_VOLTAGE_IN] = 512,
		.b[PSC_VOLTAGE_IN] = 0,
		.R[PSC_VOLTAGE_IN] = -1,
		.m[PSC_VOLTAGE_OUT] = 512,
		.b[PSC_VOLTAGE_OUT] = 0,
		.R[PSC_VOLTAGE_OUT] = -1,
		.m[PSC_CURRENT_OUT] = 5851,
		.b[PSC_CURRENT_OUT] = 0,
		.R[PSC_CURRENT_OUT] = -1,
		.m[PSC_TEMPERATURE] = 67,
		.b[PSC_TEMPERATURE] = 19655,
		.R[PSC_TEMPERATURE] = -1,
		.func[0] = PMBUS_HAVE_VIN | PMBUS_HAVE_VOUT
		         | PMBUS_HAVE_IOUT | PMBUS_HAVE_TEMP,
	},
};

static int max17616_probe(struct i2c_client *client)
{
	struct max17616_data *data;
	int rv;

	data = devm_kzalloc(&client->dev, sizeof(struct max17616_data),
			    GFP_KERNEL);
	if (!data)
		return -ENOMEM;
	data->id = i2c_match_id(max17616_id, client)->driver_data;
	data->info = max17616_info[data->id];

	return pmbus_do_probe(client, &data->info);
}

static const struct i2c_device_id max17616_id[] = {
	{"max17616", max17616},
	{}
};
MODULE_DEVICE_TABLE(i2c, max17616_id);

/* This is the driver that will be inserted */
static struct i2c_driver max17616_driver = {
	.driver = {
		   .name = "max17616",
		   },
	.probe_new = max17616_probe,
	.id_table = max17616_id,
};

module_i2c_driver(max17616_driver);

MODULE_AUTHOR("Jarvis Chen");
MODULE_DESCRIPTION("PMBus driver for MAX17616");
MODULE_LICENSE("GPL");
MODULE_IMPORT_NS(PMBUS);
