SUMMARY = "After BMC boot up ready"
PR = "r1"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

S = "${WORKDIR}"

inherit obmc-phosphor-systemd

DEPENDS = "systemd"
RDEPENDS:${PN} = "bash"

SRC_URI = " \
    file://boot-ready \
    "

SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE:${PN} = "boot-ready.service"

do_install() {
    localbindir="${D}/usr/local/bin"
    install -d ${localbindir}
    install -m 0755 ${S}/boot-ready ${localbindir}/boot-ready
}

FILES:${PN} = " ${prefix}/local/bin"
