SUMMARY = "OpenBMC logging library from facebook/openbmc"
DESCRIPTION = "Macros and functions for logging control"
SECTION = "dev"
PR = "r1"
LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://log.h;beginline=4;endline=16;md5=da35978751a9d71b73679307c4d296ec"

BBCLASSEXTEND = "native"

PV = "0.1+git${SRCPV}"

S = "${WORKDIR}/files"

SRCREV="c34232e791b37f292bf219782d3d04d663c03ae2"
SRC_URI += "git://github.com/facebook/openbmc;branch=helium;protocol=https;subpath=common/recipes-lib/log/files/"

inherit meson pkgconfig
