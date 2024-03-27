/*
 * (C) Copyright 2019-Present, Facebook, Inc.
 *
 * SPDX-License-Identifier: GPL-2.0+
 */
#include <common.h>
#include <dm.h>
#include <i2c.h>
#include <wdt.h>
#include <asm/arch/wdt.h>
#include <asm/io.h>

int watchdog_init(u32 timeout_sec)
{
#ifdef CONFIG_ASPEED_ENABLE_WATCHDOG
	struct udevice *wdt;
	int ret;
	u32 timeout_ms = timeout_sec * 1000;

	ret = uclass_first_device_err(UCLASS_WDT, &wdt);
	if (ret) {
		printf("No WDT device: %d\n", ret);
		return ret;
	}
#ifdef AST_SYSRESET_WITH_SOC
	ret = wdt_start(wdt, timeout_ms, WDT_CTRL_RESET_SOC);
#else
	ret = wdt_start(wdt, timeout_ms, WDT_CTRL_RESET_CHIP);
#endif
	if (ret) {
		printf("Start WDT%u %us failed: %d\n",
		       wdt->seq, timeout_sec, ret);
		return ret;
	}

	printf("Watchdog: %us\n", timeout_sec);
#endif
	return 0;
}

/*======================= Dual Boot Watchdog setup ===========================*/
#if defined(CONFIG_ASPEED_AST2500) || defined(CONFIG_ASPEED_AST2400)
int dual_boot_watchdog_init(uint32_t timeout_sec)
{
	struct udevice *wdt;
	int ret;
	u32 timeout_ms = timeout_sec * 1000;

	ret = uclass_get_device_by_name(UCLASS_WDT, "watchdog@1e785020", &wdt);
	if (ret) {
		printf("cannot find watchdog@1e785020(wdt2): %d\n", ret);
		return ret;
	}
#ifdef AST_SYSRESET_WITH_SOC
	ret = wdt_start(wdt, timeout_ms, WDT_CTRL_RESET_SOC);
#else
	ret = wdt_start(wdt, timeout_ms, WDT_CTRL_RESET_CHIP);
#endif
	if (ret) {
		printf("Start WDT%u %us failed: %d\n",
		       wdt->seq, timeout_sec, ret);
		return ret;
	}

	printf("Watchdog: %us\n", timeout_sec);
	return 0;
}
#endif /* CONFIG_ASPEED_AST2500 or CONFIG_ASPEED_AST2400 */


#if defined(CONFIG_ASPEED_AST2600)
#define SCU_HW_STRAPPING_REG (ASPEED_SCU_BASE + 0x510)
#define SCU_HW_CLEAR_STRAPPING_REG (ASPEED_SCU_BASE + 0x514)

#define AUTO_SOFT_RESET_CONTROL (ASPEED_FMC_BASE + 0x50)
#define WDT_CONTROL_STATUS_REG (ASPEED_FMC_BASE + 0x64)
#define WDT_TIMER_RELOAD_REG (ASPEED_FMC_BASE + 0x68)
#define WDT_CNT_RESTART_REG (ASPEED_FMC_BASE + 0x6C)

/* FMC[64] */
#define WDT_DISABLE 0x00 /* Disable WDT2 */
#define WDT_ENABLE 0x01 /* Enable WDT2 */
#define WDT_CLEAR_EVNET_COUNTER 0xEA000000 /* Clear WDT2 event counter */
#define WDT_RESTART_MAGIC 0x4755 /* Restart WDT2 timer register */
#define WDT_TIMEOUT_MAX_SECOND (0x333) /* (2^13 - 1) / 10 */

/* SCU[510] */
#define SCU_ENABLE_ABR (1 << 11) /* Enable ABR boot */
#define SCU_3B_4B_AUTO_DETECTION                                               \
	(1 << 10) /* Boot 3B/4B address mode auto detection */
#define SCU_3B_MODE_CLEAR (1 << 9) /* Boot 3B address mode auto clear */

/* FMC[50] */
#define WAIT_SPI_WIP_IDLE (1 << 1) /* Enable wait SPI WIP idel */
#define GENERATE_SOFT_RESET_COMMAND                                            \
	(1 << 0) /* Enable generate soft-reset command */


int dual_boot_watchdog_init(uint32_t timeout_sec)
{
	uint32_t reload = (timeout_sec < WDT_TIMEOUT_MAX_SECOND) ?
		(timeout_sec * 10) : (WDT_TIMEOUT_MAX_SECOND * 10) ;
	/* Enable boot SPI or eMMC ABR, and enable boot SPI 3B address mode auto-clear */
	setbits_le32(SCU_HW_STRAPPING_REG, SCU_ENABLE_ABR);
	setbits_le32(SCU_HW_CLEAR_STRAPPING_REG,
		     SCU_3B_MODE_CLEAR | SCU_3B_4B_AUTO_DETECTION);

	/* Enable Auto Soft-Reset Command Control*/
	setbits_le32(AUTO_SOFT_RESET_CONTROL, WAIT_SPI_WIP_IDLE);
	clrbits_le32(AUTO_SOFT_RESET_CONTROL, GENERATE_SOFT_RESET_COMMAND);

	/* Disable WDT */
	writel(WDT_DISABLE, WDT_CONTROL_STATUS_REG);

	/* Set the Timeout value */
	writel(reload, WDT_TIMER_RELOAD_REG);

	/* Restart watchdog timer register */
	writel(WDT_RESTART_MAGIC, WDT_CNT_RESTART_REG);

	/* Enable WDT */
	writel(WDT_ENABLE, WDT_CONTROL_STATUS_REG);

	return 0;
}

#endif /* CONFIG_ASPEED_AST2600 */
