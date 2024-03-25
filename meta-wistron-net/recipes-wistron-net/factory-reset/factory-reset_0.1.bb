SUMMARY = "Factory Reset Service"
DESCRIPTION = "Factory Reset detect Daemon."
PR = "r1"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

inherit obmc-phosphor-systemd

DEPENDS += "systemd"
RDEPENDS:${PN} += " bash phosphor-gpio-monitor"

SRC_URI = " file://obmc-factory-reset@.service \
          "

SYSTEMD_SERVICE:${PN} += "obmc-factory-reset@.service"

TMPL_GPIO = "phosphor-gpio-monitor@.service"
GPIO_TGT = "multi-user.target"

INSTFMT_FACTORY_RESET = "phosphor-gpio-monitor@factory_reset.service"
FMT_FACTORY_RESET = "../${TMPL_GPIO}:${GPIO_TGT}.requires/${INSTFMT_FACTORY_RESET}"
FACTORY_RESET_ENV_FMT = "obmc/gpio/factory_reset.conf"

INSTFMT_FACTORY_RESET = "obmc-factory-reset@.service:obmc-factory-reset@factory_reset.service"

do_install() {
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/obmc-factory-reset@.service \
                    ${D}${systemd_system_unitdir}

    factory_reset_file="${WORKDIR}/factory-reset"
    if [ -f "$factory_reset_file" ]; then
        install -d ${D}/usr/libexec/phosphor-gpio-monitor
        install -m 0777 ${WORKDIR}/factory-reset ${D}/usr/libexec/phosphor-gpio-monitor/
    fi
}
