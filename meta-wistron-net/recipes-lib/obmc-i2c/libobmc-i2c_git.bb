SUMMARY = "I2C Device Access Library from facebook/openbmc"
DESCRIPTION = "Library for accessing I2C bus and devices"
SECTION = "base"
PR = "r1"
LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://obmc-i2c.h;beginline=7;endline=19;md5=da35978751a9d71b73679307c4d296ec"

BBCLASSEXTEND = "native"

inherit meson pkgconfig

PV = "0.1+git${SRCPV}"

S = "${WORKDIR}/files"

SRCREV="c34232e791b37f292bf219782d3d04d663c03ae2"
SRC_URI += "git://github.com/facebook/openbmc;branch=helium;protocol=https;subpath=common/recipes-lib/obmc-i2c/files/"

DEPENDS += "libmisc-utils liblog"
RDEPENDS:${PN} += "libmisc-utils"

