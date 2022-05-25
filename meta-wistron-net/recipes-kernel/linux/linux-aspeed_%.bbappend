FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
KERNEL_VERSION_SANITY_SKIP="1"

LINUX_VERSION ?= "5.15.41"
SRCREV="1f7f915b33cfaac6563f267ec9164e861ba19330"

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
	"

do_kernel_prepare() {
  cp -rf ${WORKDIR}/Documentation ${STAGING_KERNEL_DIR}
	cp -rf ${WORKDIR}/drivers ${STAGING_KERNEL_DIR}
	cp -rf ${WORKDIR}/include ${STAGING_KERNEL_DIR}
}
addtask do_kernel_prepare after do_patch before do_configure
