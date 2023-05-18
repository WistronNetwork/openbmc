FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
KERNEL_VERSION_SANITY_SKIP="1"

LINUX_VERSION = "5.15.80"
SRCREV = "8600e9e279a638f3f1286d3c2dd7c45507385265"

PV = "${LINUX_VERSION}+git${SRCPV}"

SRC_URI += "\
	file://common.cfg \
	file://Documentation \
	file://drivers \
	file://include \
	file://0001-peci-Add-support-for-PECI-bus-driver-core.patch \
	file://0002-hwmon-peci-Add-support-CPUTEMP-DIMMTEMP-CPUPOWER-and.patch \
	file://0003-mfd-peci-Add-support-intel-peci-client.patch \
	file://0004-arm-dtsi-aspeed-g4-aspeed-g5-aspeed-g6-Add-PECI-adap.patch \
	file://0001-jtag-Add-support-for-aspeed-JTAG-adapter-driver.patch \
	file://0002-arm-dtsi-aspeed-g4-aspeed-g5-aspeed-g6-Add-JTAG-adap.patch \
	file://0001-hwmon-wistron-Add-support-wistron-CPLD.patch \
	file://0001-hwmon-pmbus-Add-support-xdpe132g5.patch \
	file://0001-hwmon-pmbus-xdpe12284.c-Add-a-multiple-for-VOUT.patch \
	file://0001-hwmon-pmbus-isl68137.c-Add-a-multiple-for-VOUT.patch \
	file://0001-hwmon-pmbus-Add-support-pmbus-psu.patch \
	file://0001-arm-aspeed-Add-eSPI-support.patch \
	file://0001-spi-add-user-mode-aspeed-spi-driver.patch \
	file://0001-aspeed-linux-5.15-add-shutdown-method-for-ast2500-sp.patch \
	file://0001-misc-eeprom-Add-optoe-eeprom-driver.patch \
	file://0001-mtd-spi-nor-gigadevice-Add-gd55b01ge-support.patch \
	"

do_kernel_prepare() {
  cp -rf ${WORKDIR}/Documentation ${STAGING_KERNEL_DIR}
  cp -rf ${WORKDIR}/drivers ${STAGING_KERNEL_DIR}
  cp -rf ${WORKDIR}/include ${STAGING_KERNEL_DIR}
}
addtask do_kernel_prepare after do_patch before do_configure
