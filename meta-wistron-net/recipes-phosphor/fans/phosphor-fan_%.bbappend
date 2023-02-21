FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

inherit meson

SRCREV = "e56672d5d524b8cb29cb9140612176cf506ad54a"

DEPENDS:remove = "autoconf-archive-native"

FAN_PACKAGES += " \
        ${PN}-sensor-monitor \
"

# The control, monitor, and presence apps can either be JSON or YAML driven.
PACKAGECONFIG[json] = "-Djson-config=enabled, -Djson-config=disabled"
# --------------------------------------
# ${PN}-presence-tach specific configuration
PACKAGECONFIG[presence] = "-Dpresence-service=enabled \
    -Dmachine-name=${PKG_DEFAULT_MACHINE} \
    -Dpresence-config=${STAGING_DIR_HOST}${presence_datadir}/config.yaml, \
    -Dpresence-service=disabled, \
    virtual/phosphor-fan-presence-config \
    , \
"
# --------------------------------------
# --------------------------------------
# ${PN}-control specific configuration
PACKAGECONFIG[control] = "-Dcontrol-service=enabled \
    -Dmachine-name=${PKG_DEFAULT_MACHINE} \
    -Dfan-def-yaml-file=${STAGING_DIR_HOST}${control_datadir}/fans.yaml \
    -Dfan-zone-yaml-file=${STAGING_DIR_HOST}${control_datadir}/zones.yaml \
    -Dzone-events-yaml-file=${STAGING_DIR_HOST}${control_datadir}/events.yaml \
    -Dzone-conditions-yaml-file=${STAGING_DIR_HOST}${control_datadir}/zone_conditions.yaml, \
    -Dcontrol-service=disabled, \
    virtual/phosphor-fan-control-fan-config \
    phosphor-fan-control-zone-config \
    phosphor-fan-control-events-config \
    phosphor-fan-control-zone-conditions-config \
    , \
"
# --------------------------------------
# ${PN}-monitor specific configuration
PACKAGECONFIG[monitor] = "-Dmonitor-service=enabled \
    -Dmachine-name=${PKG_DEFAULT_MACHINE} \
    -Dfan-monitor-yaml-file=${STAGING_DIR_HOST}${monitor_datadir}/monitor.yaml, \
    -Dmonitor-service=disabled, \
    phosphor-fan-monitor-config \
    , \
"
# --------------------------------------
# --------------------------------------
# phosphor-cooling-type specific configuration
PACKAGECONFIG[cooling-type] = "-Dcooling-type-service=enabled,-Dcooling-type-service=disabled,,"
# --------------------------------------
# ${PN}-sensor-monitor specific configuration
PACKAGECONFIG[sensor-monitor] = "-Dsensor-monitor-service=enabled,-Dsensor-monitor-service=disabled"


# OBMC_CHASSIS_ZERO_ONLY: hacky way to fix the templates until
# openbmc/phosphor-fan-presence#26 is resolved.  This should likely be
# returned to OBMC_CHASSIS_INSTANCES.
OBMC_CHASSIS_ZERO_ONLY = "0"

SYSTEMD_LINK:${PN}-presence-tach = " "
SYSTEMD_LINK:${PN}-presence-tach += "${@compose_list(d, 'FMT_TACH', 'OBMC_CHASSIS_ZERO_ONLY')}"
SYSTEMD_LINK:${PN}-presence-tach += "${@bb.utils.contains('PACKAGECONFIG', 'json', \
        compose_list(d, 'FMT_TACH_MUSR', 'OBMC_CHASSIS_ZERO_ONLY'), '', d)}"

SYSTEMD_LINK:${PN}-control = " "
SYSTEMD_LINK:${PN}-control += "${@bb.utils.contains('PACKAGECONFIG', 'json', \
        compose_list(d, 'FMT_CONTROL_MUSR', 'OBMC_CHASSIS_ZERO_ONLY'), \
        compose_list(d, 'FMT_CONTROL', 'OBMC_CHASSIS_ZERO_ONLY'), d)}"
SYSTEMD_LINK:${PN}-control += "${@bb.utils.contains('PACKAGECONFIG', 'json', \
        compose_list(d, 'FMT_CONTROL_PWRON', 'OBMC_CHASSIS_ZERO_ONLY'), \
        compose_list(d, 'FMT_CONTROL_INIT', 'OBMC_CHASSIS_ZERO_ONLY'), d)}"

SYSTEMD_LINK:${PN}-monitor = " "
SYSTEMD_LINK:${PN}-monitor += "${@bb.utils.contains('PACKAGECONFIG', 'json', \
                                compose_list(d, 'FMT_MONITOR_PWRON', 'OBMC_CHASSIS_ZERO_ONLY'), \
                                compose_list(d, 'FMT_MONITOR_FANSREADY', 'OBMC_CHASSIS_ZERO_ONLY'), d)}"
SYSTEMD_LINK:${PN}-monitor += "${@bb.utils.contains('PACKAGECONFIG', 'json', \
                                compose_list(d, 'FMT_MONITOR_MUSR', 'OBMC_CHASSIS_ZERO_ONLY'), \
                                compose_list(d, 'FMT_MONITOR_INIT', 'OBMC_CHASSIS_ZERO_ONLY'), d)}"

FMT_TACH = "../${TMPL_TACH}:${POWERON_TGT}.wants/${INSTFMT_TACH}"
FMT_CONTROL_PWRON = "../${TMPL_CONTROL}:${POWERON_TGT}.wants/${INSTFMT_CONTROL}"
FMT_MONITOR_PWRON = "../${TMPL_MONITOR}:${POWERON_TGT}.wants/${INSTFMT_MONITOR}"
FAN_PACKAGES:append = "${@bb.utils.contains('PACKAGECONFIG', 'sensor-monitor', ' sensor-monitor', '', d)}"

SRC_URI:append:wistron-net = " file://obmc-chassis-hard-poweroff@.target \
                               file://obmc-chassis-poweroff@0.target \
                               file://obmc-poweroff.service \
                               file://host-poweroff \
                               file://sensor-monitor.service \
                             "

PACKAGECONFIG:append = " json sensor-monitor"

EXTRA_OEMESON = "-Duse-host-power-state=enabled"
EXTRA_OEMESON:append = " -Dtests=disabled"

RDEPENDS:sensor-monitor = " bash"

do_configure:prepend:wistron-net() {
    cp -rfv ${WORKDIR}/sensor-monitor.service ${S}/sensor-monitor/service_files/sensor-monitor.service
}

do_install:append:wistron-net() {
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/obmc-chassis-hard-poweroff@.target ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/obmc-chassis-poweroff@0.target ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/obmc-poweroff.service ${D}${systemd_system_unitdir}

    install -d ${D}/usr/libexec/phosphor-fan-sensor-monitor
    install -m 0777 ${WORKDIR}/host-poweroff ${D}/usr/libexec/phosphor-fan-sensor-monitor/
}

pkg_postinst:${PN}() {
    mkdir -p $D$systemd_system_unitdir/obmc-chassis-hard-poweroff@0.target.requires
    mkdir -p $D$systemd_system_unitdir/obmc-chassis-hard-poweroff@0.target.requires/obmc-chassis-poweroff@0.target.requires

    LINK="$D$systemd_system_unitdir/obmc-chassis-hard-poweroff@0.target.requires/obmc-chassis-poweroff@0.target"
    TARGET="../obmc-chassis-poweroff@0.target"
    ln -s $TARGET $LINK

    LINK="$D$systemd_system_unitdir/obmc-chassis-hard-poweroff@0.target.requires/obmc-chassis-poweroff@0.target.requires/obmc-poweroff.service"
    TARGET="../../obmc-poweroff.service"
    ln -s $TARGET $LINK
}

FILES:sensor-monitor += "/usr/libexec/phosphor-fan-sensor-monitor/host-poweroff"

FILES:sensor-monitor += "${systemd_system_unitdir}/obmc-chassis-hard-poweroff@.target"
FILES:sensor-monitor += "${systemd_system_unitdir}/obmc-chassis-poweroff@0.target "
FILES:sensor-monitor += "${systemd_system_unitdir}/obmc-poweroff.service"
