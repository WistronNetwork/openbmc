SUMMARY = "Lattice CPLD I2C update Utility."
DESCRIPTION = "Update Lattice CPLD through I2C."
SECTION = "base"
PR = "r1"

LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://cpldupdate-i2c.c;beginline=5;endline=16;md5=69348da7e13c557a246cf7e5b163ea27"

PV = "0.1+git${SRCPV}"

S = "${WORKDIR}/files"

SRCREV="c34232e791b37f292bf219782d3d04d663c03ae2"
SRC_URI += "git://github.com/facebook/openbmc;branch=helium;protocol=https;subpath=common/recipes-utils/cpldupdate-i2c/files/ \
            file://0001-cpldupdate-i2c-Support-to-use-.jed-to-do-program.patch \
           "

do_install() {
  install -d ${D}${bindir}
  install -m 755 cpldupdate-i2c ${D}${bindir}/cpldupdate-i2c
}

LDFLAGS += "-lobmc-i2c"
DEPENDS += "libobmc-i2c"
RDEPENDS:${PN} += "libobmc-i2c"

FILES:${PN} = "${bindir}"
