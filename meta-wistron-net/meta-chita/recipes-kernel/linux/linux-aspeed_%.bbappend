FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
SRC_URI += "\
	file://platform \
	file://chita.cfg \
	file://aspeed-bmc-wistron-net-chita.dts \
	"

do_kernel_prepare:append:chita() {
  cp -rf ${WORKDIR}/platform/drivers ${STAGING_KERNEL_DIR}
}

do_compile:prepend:chita () {
  cp -rfv ${WORKDIR}/aspeed-bmc-wistron-net-chita.dts ${STAGING_KERNEL_DIR}/arch/${ARCH}/boot/dts/aspeed-bmc-wistron-net-chita.dts
}
