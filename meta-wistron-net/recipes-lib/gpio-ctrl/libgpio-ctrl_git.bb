SUMMARY = "GPIO Control Library from facebook/openbmc"
DESCRIPTION = "GPIO library for applications on openbmc kernel 4.1 or higher"
SECTION = "base"
PR = "r1"
LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://gpio.c;beginline=4;endline=16;md5=da35978751a9d71b73679307c4d296ec"

inherit meson pkgconfig
inherit ptest-meson

PV = "0.1+git${SRCPV}"

S = "${WORKDIR}/files"

SRCREV="c34232e791b37f292bf219782d3d04d663c03ae2"
SRC_URI += "git://github.com/facebook/openbmc;branch=helium;protocol=https;subpath=common/recipes-lib/gpio-ctrl/files/"

DEPENDS += "libmisc-utils libobmc-i2c gtest gmock"
RDEPENDS:${PN} += " libmisc-utils libobmc-i2c"
RDEPENDS:${PN}-ptest += "libmisc-utils libobmc-i2c"
