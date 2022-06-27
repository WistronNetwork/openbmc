SUMMARY = "IPMI FRU parse Utility"
SECTION = "utils"
LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://LICENSE;md5=a23a74b3f4caf9616230789d94217acb"

PV = "0.1+git${SRCPV}"
SRCREV="3371b0a5f30ffc9e2d4095795984514cb519623f"

SRC_URI += "git://github.com/duanzhang512/ipmi-fru-parser.git;branch=master;protocol=https \
            file://Makefile \
           "

S = "${WORKDIR}/git"

TARGET_CC_ARCH += "${LDFLAGS}"

do_compile:prepend() {
  cp -rfv ${WORKDIR}/Makefile ${S}
}

do_install() {
  bin="${D}/usr/local/bin"
  install -d ${bin}
  install -m 0755 ipmi-fru-parser ${bin}/ipmi-fru-parser
}

FILES:${PN} = "/usr/local/bin"

FILES:${PN}-dbg += "/usr/local/bin/.debug"
