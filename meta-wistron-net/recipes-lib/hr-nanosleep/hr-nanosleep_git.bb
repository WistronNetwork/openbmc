SUMMARY = "High-resolution nanosleep from facebook/openbmc"
DESCRIPTION = "High resolution nanosleep that spins on CPU if the time the sleep is too small"
SECTION = "dev"
LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://hr_nanosleep.h;beginline=4;endline=16;md5=da35978751a9d71b73679307c4d296ec"

S = "${WORKDIR}/files"

SRCREV="c34232e791b37f292bf219782d3d04d663c03ae2"
SRC_URI += "git://github.com/facebook/openbmc;branch=helium;protocol=https;subpath=common/recipes-lib/hr-nanosleep/files/"

do_install() {
  # common lib and include files
  install -d ${D}${includedir}/openbmc
  install -m 0644 hr_nanosleep.h ${D}${includedir}/openbmc/hr_nanosleep.h
}

FILES:${PN}-dev = "${includedir}/openbmc/hr_nanosleep.h"
