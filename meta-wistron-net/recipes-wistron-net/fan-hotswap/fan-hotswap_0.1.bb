SUMMARY = "Fan Hotswap Service"
DESCRIPTION = "Fan Detect Hotswap Daemon."
PR = "r1"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

inherit obmc-phosphor-systemd

DEPENDS += "systemd"
RDEPENDS:${PN} += " bash phosphor-gpio-monitor-monitor"

SRC_URI = " file://obmc-fan-hotswap@.service \
          "

SYSTEMD_SERVICE:${PN} += "obmc-fan-hotswap@.service"

FAN_INSTANCES = ""

TMPL_GPIO = "phosphor-gpio-monitor@.service"
GPIO_TGT = "multi-user.target"

INSTFMT_FAN_PRESENT = "phosphor-gpio-monitor@fan{0}.service"
FMT_FAN_PRESENT = "../${TMPL_GPIO}:${GPIO_TGT}.requires/${INSTFMT_FAN_PRESENT}"
FAN_PRESENT_ENV_FMT = "obmc/gpio/fan{0}.conf"
FAN_PRESENT_CONF_FMT = "obmc-fan-hotswap.conf:phosphor-gpio-monitor@fan{0}.service.d/obmc-fan-hotswap.conf"
SYSTEMD_LINK:${PN} += " ${@compose_list(d, 'FMT_FAN_PRESENT', 'FAN_INSTANCES')}"
SYSTEMD_ENVIRONMENT_FILE:${PN} = " ${@compose_list(d, 'FAN_PRESENT_ENV_FMT', 'FAN_INSTANCES')}"
SYSTEMD_OVERRIDE:${PN} += " ${@compose_list_zip(d, 'FAN_PRESENT_CONF_FMT', 'FAN_INSTANCES')}"

INSTFMT_FAN_HOTSWAP = "obmc-fan-hotswap@.service:obmc-fan-hotswap@fan{0}.service"
SYSTEMD_LINK:${PN} += " ${@compose_list(d, 'INSTFMT_FAN_HOTSWAP', 'FAN_INSTANCES')}"

do_install() {
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/obmc-fan-hotswap@.service \
                    ${D}${systemd_system_unitdir}

    fan_hotswap_file="${WORKDIR}/fan-hotswap"
    if [ -f "$fan_hotswap_file" ]; then
        install -d ${D}/usr/libexec/phosphor-gpio-monitor
        install -m 0777 ${WORKDIR}/fan-hotswap ${D}/usr/libexec/phosphor-gpio-monitor/
    fi


    if [ -n "$FAN_INSTANCES" ]; then
        for i in $FAN_INSTANCES
        do
            override_dir="${D}${systemd_system_unitdir}/phosphor-gpio-monitor@fan${i}.service.d"
            mkdir -p ${override_dir}
        done
    fi
}
