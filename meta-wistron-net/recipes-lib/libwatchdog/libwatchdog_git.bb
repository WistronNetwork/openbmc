SUMMARY = "Watchdog Control Library from facebook/openbmc"
DESCRIPTION = "watchdog control library for apps running in kernel 4.17 or higher versions"
SECTION = "base"
PR = "r1"
LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://src/watchdog.c;beginline=4;endline=16;md5=da35978751a9d71b73679307c4d296ec"

inherit meson pkgconfig

PV = "0.2+git${SRCPV}"

S = "${WORKDIR}/watchdog/libwatchdog-0.2"

SRCREV="c34232e791b37f292bf219782d3d04d663c03ae2"
SRC_URI += "git://github.com/facebook/openbmc;branch=helium;protocol=https;subpath=common/recipes-lib/watchdog"

do_configure:prepend() {
  cp -rfv ${WORKDIR}/watchdog/libwatchdog/src ${S}
}
