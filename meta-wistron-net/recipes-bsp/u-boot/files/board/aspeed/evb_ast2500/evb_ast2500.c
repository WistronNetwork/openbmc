/*
 * Copyright 2016 IBM Corporation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */
#include <common.h>
#include <dm.h>
#include <ram.h>
#include <timer.h>
#include <asm/io.h>
#include <asm/arch/platform.h>
#include <asm/arch/scu_ast2500.h>
#include <asm/arch/sdram_ast2500.h>
#include <asm/arch/timer.h>
#include <linux/err.h>
#include <dm/uclass.h>
#include <i2c.h>

#include "util.h"

DECLARE_GLOBAL_DATA_PTR;

#define AST_LPC_HICR5 0x080
#define LPC_HICR5_ENFWH BIT(10)
#define AST_LPC_HICRB 0x100
#define LPC_HICRB_SIO_ILPC2AHB_DIS BIT(6)

#define AST_SDMC_PROTECT 0x00
#define AST_SDMC_GFX_PROT 0x08
#define SDMC_GFX_PROT_VGA_CURSOR BIT(0)
#define SDMC_GFX_PROT_VGA_CG_READ BIT(1)
#define SDMC_GFX_PROT_VGA_ASCII_READ BIT(2)
#define SDMC_GFX_PROT_VGA_CRT BIT(3)
#define SDMC_GFX_PROT_PCIE BIT(16)
#define SDMC_GFX_PROT_XDMA BIT(17)

static void isolate_bmc(void)
{
	bool sdmc_unlocked;
	u32 val;

	/* iLPC2AHB */
#if !defined(CONFIG_ASPEED_ENABLE_SUPERIO)
	val = readl(ASPEED_HW_STRAP1);
	val |= SCU_HWSTRAP_LPC_SIO_DEC_DIS;
	writel(val, ASPEED_HW_STRAP1);
#endif

	val = readl(ASPEED_LPC_CTRL + AST_LPC_HICRB);
	val |= LPC_HICRB_SIO_ILPC2AHB_DIS;
	writel(val, ASPEED_LPC_CTRL + AST_LPC_HICRB);

	/* P2A, PCIe BMC */
	val = readl(ASPEED_PCIE_CONFIG_SET);
	val &= ~(SCU_PCIE_CONFIG_SET_BMC_DMA
						| SCU_PCIE_CONFIG_SET_BMC_MMIO
						| SCU_PCIE_CONFIG_SET_BMC_EN
						| SCU_PCIE_CONFIG_SET_VGA_MMIO);
	writel(val, ASPEED_PCIE_CONFIG_SET);

	/* Debug UART */
#if !defined(CONFIG_ASPEED_ENABLE_DEBUG_UART)
	val = readl(ASPEED_MISC1_CTRL);
	val |= SCU_MISC_DEBUG_UART_DISABLE;
	writel(val, ASPEED_MISC1_CTRL);
#endif

	/* X-DMA */
	sdmc_unlocked = readl(ASPEED_SDRAM_CTRL + AST_SDMC_PROTECT);
	if (!sdmc_unlocked)
		writel(SDRAM_UNLOCK_KEY, ASPEED_SDRAM_CTRL + AST_SDMC_PROTECT);

	val = readl(ASPEED_SDRAM_CTRL + AST_SDMC_GFX_PROT);
	val |= (SDMC_GFX_PROT_VGA_CURSOR
					| SDMC_GFX_PROT_VGA_CG_READ
					| SDMC_GFX_PROT_VGA_ASCII_READ
					| SDMC_GFX_PROT_VGA_CRT
					| SDMC_GFX_PROT_PCIE
					| SDMC_GFX_PROT_XDMA);
	writel(val, ASPEED_SDRAM_CTRL + AST_SDMC_GFX_PROT);

	if (!sdmc_unlocked)
		writel(~SDRAM_UNLOCK_KEY, ASPEED_SDRAM_CTRL + AST_SDMC_PROTECT);

	/* LPC2AHB */
	val = readl(ASPEED_LPC_CTRL + AST_LPC_HICR5);
	val &= ~LPC_HICR5_ENFWH;
	writel(val, ASPEED_LPC_CTRL + AST_LPC_HICR5);
}

int board_init(void)
{
	struct udevice *dev;
	int i;
	int ret;

#if CONFIG_IS_ENABLED(ASPEED_ENABLE_DUAL_BOOT_WATCHDOG)
	dual_boot_watchdog_init(CONFIG_ASPEED_WATCHDOG_TIMEOUT);
#else
	watchdog_init(CONFIG_ASPEED_WATCHDOG_TIMEOUT);
#endif

	isolate_bmc();

	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;

	/*
		* Loop over all MISC uclass drivers to call the comphy code
		* and init all CP110 devices enabled in the DT
		*/
	i = 0;
	while (1) {
		/* Call the comphy code via the MISC uclass driver */
		ret = uclass_get_device(UCLASS_MISC, i++, &dev);

		/* We're done, once no further CP110 device is found */
		if (ret)
			break;
	}

	return 0;
}
