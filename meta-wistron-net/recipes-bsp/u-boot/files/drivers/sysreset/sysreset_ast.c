// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2016 Google, Inc
 */
#include <common.h>
#include <dm.h>
#include <errno.h>
#include <sysreset.h>
#include <wdt.h>
#include <asm/io.h>
#include <asm/arch/wdt.h>
#include <linux/err.h>

static int ast_sysreset_request(struct udevice *dev, enum sysreset_t type)
{
	struct udevice *wdt;
	u32 reset_mode;
	int ret = uclass_first_device(UCLASS_WDT, &wdt);

	dev_info(dev, "%s( type=%d )\n", __func__, type);
	if (ret)
		return ret;

	switch (type) {
	case SYSRESET_WARM:
		reset_mode = WDT_CTRL_RESET_CPU;
		break;
	case SYSRESET_COLD:
#ifdef AST_SYSRESET_WITH_SOC
		dev_info(dev, "SOC reset with mask=0x%X\n", AST_SYSRESET_WITH_SOC);
		reset_mode = WDT_CTRL_RESET_SOC | (AST_SYSRESET_WITH_SOC << 2);
#else
		dev_info(dev, "Full-chip reset\n");
		reset_mode = WDT_CTRL_RESET_CHIP;
#endif
		break;
	default:
		return -EPROTONOSUPPORT;
	}

	ret = wdt_expire_now(wdt, reset_mode);
	if (ret) {
		dev_err(dev, "Sysreset failed: %d", ret);
		return ret;
	}

	return -EINPROGRESS;
}

static struct sysreset_ops ast_sysreset = {
	.request	= ast_sysreset_request,
};

U_BOOT_DRIVER(sysreset_ast) = {
	.name	= "ast_sysreset",
	.id	= UCLASS_SYSRESET,
	.ops	= &ast_sysreset,
};
