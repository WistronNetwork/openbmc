SUMMARY = "ALTERA Jam STAPL Byte-Code Player from facebook/openbmc"
SECTION = "utils"
LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://COPYING;md5=eb723b61539feef013de476e68b5c50a"

PV = "0.1+git${SRCPV}"

S = "${WORKDIR}/files/code"

SRCREV="c34232e791b37f292bf219782d3d04d663c03ae2"
SRC_URI += "git://github.com/facebook/openbmc;branch=helium;protocol=https;subpath=common/recipes-utils/jbi/files/"

LDFLAGS += "-llog -lgpio-ctrl"
DEPENDS += "hr-nanosleep liblog libgpio-ctrl"
RDEPENDS:${PN} += "libgpio-ctrl liblog"

do_install() {
  bin="${D}/usr/local/bin"
  install -d ${bin}
  install -m 755 jbi ${bin}/jbi
}

FILES:${PN} = "/usr/local/bin"

FILES:${PN}-dbg += "/usr/local/bin/.debug"
