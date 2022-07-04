SUMMARY = "CPLD Update Utilities from facebook/openbmc"
DESCRIPTION = "Util for CPLD update"
SECTION = "base"
PR = "r1"

LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://COPYING;md5=eb723b61539feef013de476e68b5c50a"

PV = "0.1+git${SRCPV}"

S = "${WORKDIR}/files"

SRCREV="c34232e791b37f292bf219782d3d04d663c03ae2"
SRC_URI += "git://github.com/facebook/openbmc;branch=helium;protocol=https;subpath=common/recipes-utils/cpldupdate/files/"

do_install() {
	install -d ${D}${bindir}
	install -m 0755 ispvm ${D}${bindir}/ispvm
}

LDFLAGS += "-lobmc-i2c"
DEPENDS += "libcpldupdate-dll-helper libobmc-i2c"
RDEPENDS:${PN} += "libobmc-i2c"
