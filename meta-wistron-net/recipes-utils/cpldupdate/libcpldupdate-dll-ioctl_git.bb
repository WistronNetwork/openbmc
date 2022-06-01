SUMMARY = "CPLD update IOCTL DLL from facebook/openbmc"
DESCRIPTION = "CPLD update IOCTL DLL"
SECTION = "dev"

LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://cpldupdate_dll_ioctl.c;beginline=4;endline=16;md5=da35978751a9d71b73679307c4d296ec"

S = "${WORKDIR}/files/lib/ioctl"

SRCREV="c34232e791b37f292bf219782d3d04d663c03ae2"
SRC_URI += "git://github.com/facebook/openbmc;branch=helium;protocol=https;subpath=common/recipes-utils/cpldupdate/files/"

inherit cmake

DEPENDS += "libcpldupdate-dll-helper"

# The shared library is used used as package instead of -dev
FILES_SOLIBSDEV = ""
FILES:${PN} += "usr/lib"
