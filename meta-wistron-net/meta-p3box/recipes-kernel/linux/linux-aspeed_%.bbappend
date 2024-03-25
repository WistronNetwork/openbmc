FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
SRC_URI += "\
	file://platform \
	file://p3box.cfg \
	file://aspeed-bmc-wistron-net-p3box.dts \
	"

do_kernel_prepare:append() {
  cp -rf ${WORKDIR}/platform/drivers ${STAGING_KERNEL_DIR}
}

do_compile:prepend() {
  cp -rfv ${WORKDIR}/aspeed-bmc-wistron-net-p3box.dts ${STAGING_KERNEL_DIR}/arch/${ARCH}/boot/dts/aspeed-bmc-wistron-net-p3box.dts
}
