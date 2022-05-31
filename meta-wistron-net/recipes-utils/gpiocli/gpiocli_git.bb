SUMMARY = "GPIO Control Command Line Interface from facebook/openbmc"
DESCRIPTION = "GPIO Control Command Line Interface"
SECTION = "base"
PR = "r1"
LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://gpiocli.c;beginline=4;endline=16;md5=da35978751a9d71b73679307c4d296ec"

S = "${WORKDIR}/files"

SRCREV="c34232e791b37f292bf219782d3d04d663c03ae2"
SRC_URI += "git://github.com/facebook/openbmc;branch=helium;protocol=https;subpath=common/recipes-utils/gpiocli/files/"

pkgdir = "gpiocli"
binfiles = "gpiocli \
           "

CFLAGS += "-Wall -Werror "
LDFLAGS += " -lgpio-ctrl -lmisc-utils"
RDEPENDS:${PN} += " libgpio-ctrl libmisc-utils"
DEPENDS:append = " libgpio-ctrl libmisc-utils"

do_install() {
  dst="${D}/usr/local/fbpackages/${pkgdir}"
  bin="${D}/usr/local/bin"
  install -d $dst
  install -d $bin
  for f in ${binfiles}; do
    install -m 755 $f ${dst}/$f
    ln -snf ../fbpackages/${pkgdir}/$f ${bin}/$f
  done
}

FBPACKAGEDIR = "${prefix}/local/fbpackages"
FILES:${PN} = "${FBPACKAGEDIR}/gpiocli ${prefix}/local/bin"
