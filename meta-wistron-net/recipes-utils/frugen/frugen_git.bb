SUMMARY = "IPMI FRU Generator Utility"
DESCRIPTION = "Utility to generate IPMI FRU binary"
SECTION = "base"
PR = "r1"
LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://README.md;beginline=5;endline=7;md5=e9dc6352a00dfe684f975f8534c65f7f"

inherit cmake

SRC_URI = "git://github.com/ipmitool/frugen \
          "

DEPENDS = "json-c"

PV = "0.1+git${SRCPV}"
SRCREV = "e30bb9cd6e76fb790a9aa67d544cdb2f0fdb5da6"

S = "${WORKDIR}/git"

do_install() {
  bin="${D}/usr/local/bin"
  install -d ${bin}
  install -m 0755 frugen-static ${bin}/frugen
}

FILES:${PN} = "/usr/local/bin"

INHIBIT:PACKAGE:DEBUG:SPLIT = "1"
INHIBIT:PACKAGE:STRIP = "1"
