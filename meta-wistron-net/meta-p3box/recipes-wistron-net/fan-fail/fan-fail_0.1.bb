SUMMARY = "Fan Fail Service"
DESCRIPTION = "Fan Fail Detect Daemon."
PR = "r1"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

inherit obmc-phosphor-systemd

DEPENDS += "systemd"
RDEPENDS:${PN} += " bash phosphor-gpio-monitor-monitor"

SRC_URI = " file://obmc-fan-fail@.service \
            file://fan-fail \
          "

SYSTEMD_SERVICE:${PN} += "obmc-fan-fail@.service"

FAN_INSTANCES = "all"

TMPL_GPIO = "phosphor-gpio-monitor@.service"
GPIO_TGT = "multi-user.target"

INSTFMT_FAN_DETECT = "phosphor-gpio-monitor@fanfail_{0}.service"
FMT_FAN_DETECT = "../${TMPL_GPIO}:${GPIO_TGT}.requires/${INSTFMT_FAN_DETECT}"
FAN_DETECT_ENV_FMT = "obmc/gpio/fanfail_{0}.conf"
SYSTEMD_LINK:${PN} += " ${@compose_list(d, 'FMT_FAN_DETECT', 'FAN_INSTANCES')}"
SYSTEMD_ENVIRONMENT_FILE:${PN} = " ${@compose_list(d, 'FAN_DETECT_ENV_FMT', 'FAN_INSTANCES')}"

INSTFMT_FAN_FAIL = "obmc-fan-fail@.service:obmc-fan-fail@fanfail_{0}.service"
SYSTEMD_LINK:${PN} += " ${@compose_list(d, 'INSTFMT_FAN_FAIL', 'FAN_INSTANCES')}"

do_install() {
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/obmc-fan-fail@.service \
                    ${D}${systemd_system_unitdir}

    fan_fail_file="${WORKDIR}/fan-fail"
    if [ -f "$fan_fail_file" ]; then
        install -d ${D}/usr/libexec/phosphor-gpio-monitor
        install -m 0777 ${WORKDIR}/fan-fail ${D}/usr/libexec/phosphor-gpio-monitor/
    fi
}
