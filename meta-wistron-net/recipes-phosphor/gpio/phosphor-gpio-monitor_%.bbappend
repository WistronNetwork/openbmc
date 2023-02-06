FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRCREV = "b5bd88532ccef5e82810a1472af3d8a9213873ae"

SRC_URI:append = " file://wistron-gpio-monitor@.service"
SRC_URI:append = " file://fan-hotswap/obmc-fan-hotswap.service"

RDEPENDS:${PN} += "bash"

SYSTEMD_SERVICE:${PN} += "obmc-fan-hotswap.service"

TMPL_GPIO = "phosphor-gpio-monitor@.service"
GPIO_TGT = "multi-user.target"

INSTFMT_FAN = "phosphor-gpio-monitor@fan{0}.service"
FMT_FAN = "../${TMPL_GPIO}:${GPIO_TGT}.requires/${INSTFMT_FAN}"
FAN_ENV_FMT = "obmc/gpio/fan{0}.conf"
OBMC_FAN_INSTANCES = ""

SYSTEMD_LINK:${PN}-monitor:append = " ${@compose_list(d, 'FMT_FAN', 'OBMC_FAN_INSTANCES')}"
SYSTEMD_ENVIRONMENT_FILE:${PN}-monitor:append = " ${@compose_list(d, 'FAN_ENV_FMT', 'OBMC_FAN_INSTANCES')}"

do_configure:append() {
    cp -rfv ${WORKDIR}/wistron-gpio-monitor@.service ${S}/phosphor-gpio-monitor@.service
}

do_install:append() {
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/fan-hotswap/obmc-fan-hotswap.service \
                    ${D}${systemd_system_unitdir}

    fan_hotswap_file="${WORKDIR}/fan-hotswap.sh"
    if [ -f "$fan_hotswap_file" ]; then
        install -d ${D}/usr/libexec/phosphor-gpio-monitor
        install -m 0777 ${WORKDIR}/fan-hotswap.sh ${D}/usr/libexec/phosphor-gpio-monitor/
    fi
}
FILES:${PN}-monitor += "/lib/systemd/system/obmc-fan-hotswap.service"
