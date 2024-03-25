SUMMARY = "Setup I2C Mux Selection And Re-bind I2C Driver"
PR = "r1"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

S = "${WORKDIR}"

inherit obmc-phosphor-systemd

DEPENDS = "systemd"
RDEPENDS:${PN} = "bash"

SRC_URI = "file://i2c-mux-setup \
           file://i2c-mux-setup.service \
          "

SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE:${PN} = " i2c-mux-setup.service"

do_install() {
    localbindir="${D}/usr/local/bin"
    install -d ${localbindir}
    install -m 755 i2c-mux-setup ${localbindir}/i2c-mux-setup
}

FILES:${PN} = " ${prefix}/local/bin"
