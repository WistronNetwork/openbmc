SUMMARY = "PSU Hotswap Service"
DESCRIPTION = "PSU Detect Hotswap Daemon."
PR = "r1"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

inherit obmc-phosphor-systemd

DEPENDS += "systemd"
RDEPENDS:${PN} += " bash phosphor-gpio-monitor-monitor"

SRC_URI = " file://obmc-psu-hotswap@.service \
          "

SYSTEMD_SERVICE:${PN} += "obmc-psu-hotswap@.service"

PSU_INSTANCES = ""

TMPL_GPIO = "phosphor-gpio-monitor@.service"
GPIO_TGT = "multi-user.target"

INSTFMT_PSU_PRESENT = "phosphor-gpio-monitor@psu{0}.service"
FMT_PSU_PRESENT = "../${TMPL_GPIO}:${GPIO_TGT}.requires/${INSTFMT_PSU_PRESENT}"
PSU_PRESENT_ENV_FMT = "obmc/gpio/psu{0}.conf"
SYSTEMD_LINK:${PN} += " ${@compose_list(d, 'FMT_PSU_PRESENT', 'PSU_INSTANCES')}"
SYSTEMD_ENVIRONMENT_FILE:${PN} = " ${@compose_list(d, 'PSU_PRESENT_ENV_FMT', 'PSU_INSTANCES')}"

INSTFMT_PSU_HOTSWAP = "obmc-psu-hotswap@.service:obmc-psu-hotswap@psu{0}.service"
SYSTEMD_LINK:${PN} += " ${@compose_list(d, 'INSTFMT_PSU_HOTSWAP', 'PSU_INSTANCES')}"

do_install() {
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/obmc-psu-hotswap@.service \
                    ${D}${systemd_system_unitdir}

    psu_hotswap_file="${WORKDIR}/psu-hotswap"
    if [ -f "$psu_hotswap_file" ]; then
        install -d ${D}/usr/libexec/phosphor-gpio-monitor
        install -m 0777 ${WORKDIR}/psu-hotswap ${D}/usr/libexec/phosphor-gpio-monitor/
    fi
}
