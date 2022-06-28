SUMMARY = "Watchdog Control Command Line Interface from facebook/openbmc"
DESCRIPTION = "Watchdog Control Command Line Interface"
SECTION = "base"
PR = "r1"
LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://wdtcli.c;beginline=7;endline=19;md5=da35978751a9d71b73679307c4d296ec"

PV = "0.1+git${SRCPV}"

S = "${WORKDIR}/files"

SRCREV="c34232e791b37f292bf219782d3d04d663c03ae2"
SRC_URI += "git://github.com/facebook/openbmc;branch=helium;protocol=https;subpath=common/recipes-utils/wdtcli/files/"

pkgdir = "wdtcli"
binfiles = "wdtcli \
           "

CFLAGS += "-Wall -Werror "
LDFLAGS += " -lwatchdog"
RDEPENDS:${PN} += " libwatchdog"
DEPENDS:append = " libwatchdog"

do_install() {
  dst="${D}/usr/local/fbpackages/${pkgdir}"
  bin="${D}/usr/local/bin"
  install -d $dst
  install -d $bin


  for f in ${binfiles}; do
    install -m 755 $f ${dst}/$f
    echo $(pwd) > test_file
    ln -snf ../fbpackages/${pkgdir}/$f ${bin}/$f
  done
}

FBPACKAGEDIR = "${prefix}/local/fbpackages"
FILES:${PN} = "${FBPACKAGEDIR}/wdtcli ${prefix}/local/bin"
