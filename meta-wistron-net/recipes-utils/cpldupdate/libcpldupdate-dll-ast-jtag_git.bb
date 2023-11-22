SUMMARY = "CPLD update DLL using Aspeed JTAG"
DESCRIPTION = "CPLD update DLL using Aspeed JTAG"
SECTION = "dev"

LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://cpldupdate_dll_ast_jtag.c;beginline=4;endline=16;md5=da35978751a9d71b73679307c4d296ec"

PV = "0.1+git${SRCPV}"

S = "${WORKDIR}/files"

SRCREV="c34232e791b37f292bf219782d3d04d663c03ae2"
SRC_URI += "git://github.com/facebook/openbmc;branch=helium;protocol=https;subpath=meta-aspeed/recipes-utils/cpldupdate/files/"

inherit cmake

DEPENDS += "libcpldupdate-dll-helper liblog libmisc-utils"
RDEPENDS:${PN} += "liblog libmisc-utils"

# The shared library is used used as package instead of -dev
FILES_SOLIBSDEV = ""
FILES:${PN} += "usr/lib"
