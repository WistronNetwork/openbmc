FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
KERNEL_VERSION_SANITY_SKIP="1"

LINUX_VERSION = "5.15.61"
SRCREV = "03008ae8e19dd018de4a3f818de4fa287721de38"

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
	"

do_kernel_prepare() {
  cp -rf ${WORKDIR}/Documentation ${STAGING_KERNEL_DIR}
  cp -rf ${WORKDIR}/drivers ${STAGING_KERNEL_DIR}
  cp -rf ${WORKDIR}/include ${STAGING_KERNEL_DIR}
}
addtask do_kernel_prepare after do_patch before do_configure
