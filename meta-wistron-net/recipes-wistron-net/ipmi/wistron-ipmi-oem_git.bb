SUMMARY = "Wistron net OEM IPMI commands"
DESCRIPTION = "Wistron net OEM IPMI commands"

LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

PV = "0.1+git${SRCPV}"

S = "${WORKDIR}/git"

DEPENDS = "boost phosphor-ipmi-host phosphor-logging systemd libgpiod libobmc-i2c libgpio-ctrl"

inherit cmake pkgconfig obmc-phosphor-ipmiprovider-symlink

EXTRA_OECMAKE = "-DENABLE_TEST=0 -DYOCTO=1"

LIBRARY_NAMES = "libwistronoemcmds.so"

SRC_URI = "git://git@10.31.80.71/justine_team/wistron-net-ipmi-oem.git;branch=master;protocol=ssh"
SRCREV = "250ee1db8be7cf4763e3010d0ac6c13042deface"

HOSTIPMI_PROVIDER_LIBRARY += "${LIBRARY_NAMES}"
NETIPMI_PROVIDER_LIBRARY += "${LIBRARY_NAMES}"

FILES:${PN}:append = " ${libdir}/ipmid-providers/lib*${SOLIBS}"
FILES:${PN}:append = " ${libdir}/host-ipmid/lib*${SOLIBS}"
FILES:${PN}:append = " ${libdir}/net-ipmid/lib*${SOLIBS}"
FILES:${PN}-dev:append = " ${libdir}/ipmid-providers/lib*${SOLIBSDEV}"

do_configure:prepend() {
    cp -rfv ${S}/fru/${MACHINE}_fru.hpp ${S}/include/fru.hpp
}
