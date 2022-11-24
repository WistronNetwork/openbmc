FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRCREV = "4a105cd64232ab66cef8461dac0302aa9021d9da"

SRC_URI:append = " \
    file://0001-Set-MAC-into-eeprom.patch \
    file://0002-Setup-sensor-id-type-to-8-bit-ASCII.patch \
    file://0003-comment-out-initSELCache-in-register-function.patch \
    file://0004-Add-sensorReadingType-in-GetSensorType-command.patch \
    file://phosphor-ipmi-host.service \
    file://transporthandler_oem.cpp \
    "

inherit meson

PACKAGECONFIG = "allowlist boot-flag-safe-mode softoff libuserlayer transport-oem sel-logger-clears-sel"
PACKAGECONFIG[dynamic-sensors] = "-Ddynamic-sensors=enabled,-Ddynamic-sensors=disabled"
PACKAGECONFIG[hybrid-sensors] = "-Dhybrid-sensors=enabled,-Dhybrid-sensors=disabled"
PACKAGECONFIG[sel-logger-clears-sel] = "-Dsel-logger-clears-sel=enabled,-Dsel-logger-clears-sel=disabled"
PACKAGECONFIG[allowlist] = "-Dipmi-whitelist=enabled,-Dipmi-whitelist=disabled"
PACKAGECONFIG[i2c-allowlist] = "-Di2c-whitelist-check=enabled,-Di2c-whitelist-check=disabled"
PACKAGECONFIG[transport-oem] = "-Dtransport-oem=enabled,-Dtransport-oem=disabled"
PACKAGECONFIG[boot-flag-safe-mode] = "-Dboot-flag-safe-mode-support=enabled,-Dboot-flag-safe-mode-support=disabled"
PACKAGECONFIG[softoff] = "-Dsoftoff=enabled,-Dsoftoff=disabled"
PACKAGECONFIG[update-functional-on-fail] = "-Dupdate-functional-on-fail=enabled,-Dupdate-functional-on-fail=disabled"
PACKAGECONFIG[libuserlayer] = "-Dlibuserlayer=enabled,-Dlibuserlayer=disabled"
PACKAGECONFIG[sensors-cache] = "-Dsensors-cache=enabled,-Dsensors-cache=disabled"

FILES:${PN}:append = " ${systemd_system_unitdir}/phosphor-ipmi-host.service.d/*.conf"

EXTRA_OEMESON = " \
        -Dsensor-yaml-gen=${STAGING_DIR_NATIVE}${sensor_datadir}/sensor.yaml \
        -Dinvsensor-yaml-gen=${STAGING_DIR_NATIVE}${sensor_datadir}/invsensor.yaml \
        -Dfru-yaml-gen=${STAGING_DIR_NATIVE}${config_datadir}/fru_config.yaml \
        "
EXTRA_OEMESON:append = " \
        -Dwhitelist-conf="${WHITELIST_CONF}" \
        "

EXTRA_OEMESON:append = " -Dtests=disabled"

IPMI_HOST_NEEDED_SERVICES = "\
    mapper-wait@-xyz-openbmc_project-control-host{}-boot.service \
    mapper-wait@-xyz-openbmc_project-control-host{}-boot-one_time.service \
    mapper-wait@-xyz-openbmc_project-control-host{}-power_restore_policy.service \
    mapper-wait@-xyz-openbmc_project-control-host{}-restriction_mode.service \
    "

do_configure:prepend() {
    cp -rfv ${WORKDIR}/transporthandler_oem.cpp ${S}
}

do_install:append() {
    # Create service override file.
    override_dir=${D}${systemd_system_unitdir}/phosphor-ipmi-host.service.d
    override_file=${override_dir}/10-override.conf
    mkdir -p ${override_dir}
    echo "[Unit]" > ${override_file}

    # Insert host-instance based service dependencies.
    for i in ${OBMC_HOST_INSTANCES};
    do
        for s in ${IPMI_HOST_NEEDED_SERVICES};
        do
            service=$(echo ${s} | sed "s/{}/${i}/g")
            echo "Wants=${service}" >> ${override_file}
            echo "After=${service}" >> ${override_file}
        done
    done

    rm -f ${S}/transporthandler_oem.cpp
}
