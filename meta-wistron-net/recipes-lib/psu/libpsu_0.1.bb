SUMMARY = "PSU Library"
DESCRIPTION = "Library for accessing PSU device"
SECTION = "base"
PR = "r1"
LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://${WORKDIR}/psu.c;beginline=2;endline=4;md5=318cd43e9a544fc544d594196e1d2c47"

BBCLASSEXTEND = "native"

inherit meson pkgconfig

S = "${WORKDIR}"

SRC_URI += "file://meson.build \
            file://psu.c \
            file://psu.h \
           "

DEPENDS += "libmisc-utils liblog libobmc-i2c"
RDEPENDS:${PN} += "libmisc-utils liblog libobmc-i2c"
