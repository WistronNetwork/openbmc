SUMMARY = "BMC MAC Address Setup Service"
PR = "r1"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

S = "${WORKDIR}"

inherit obmc-phosphor-systemd

DEPENDS = "systemd"
RDEPENDS:${PN} = "bash"

SRC_URI = "file://bmc-mac-setup \
           file://bmc-mac-setup.service \
          "

SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE:${PN} = " bmc-mac-setup.service"

do_install() {
    localbindir="${D}/usr/local/bin"
    install -d ${localbindir}
    install -m 755 bmc-mac-setup ${localbindir}/bmc-mac-setup

    bmc_envdir="obmc/bmc-mac"
    bmc_envfile="${bmc_envdir}/mac-address.conf"
    if [ -f "$bmc_envfile" ]; then
        bmc_env="${D}/etc/default/${bmc_envdir}"
        install -d ${bmc_env}
        install -m 644 ${bmc_envfile} ${D}/etc/default/${bmc_envfile}
    fi
}

FILES:${PN} = " ${prefix}/local/bin /etc/default"
