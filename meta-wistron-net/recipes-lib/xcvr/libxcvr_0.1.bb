SUMMARY = "XCVR Library"
DESCRIPTION = "XCVR Library for accessing XCVR device"
SECTION = "base"
PR = "r1"
LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://${WORKDIR}/xcvr-optoe.cpp;beginline=2;endline=4;md5=318cd43e9a544fc544d594196e1d2c47"

BBCLASSEXTEND = "native"

inherit meson pkgconfig

S = "${WORKDIR}"

SRC_URI += "file://meson.build \
            file://xcvr-optoe.cpp \
            file://xcvr-optoe.hpp \
           "

DEPENDS += "libmisc-utils"
RDEPENDS:${PN} += "libmisc-utils"
