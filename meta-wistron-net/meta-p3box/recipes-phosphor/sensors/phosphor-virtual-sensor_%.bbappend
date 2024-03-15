FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append:p3box += " \
            file://virtual_sensor_config.json \
           "

do_install:append:p3box() {
    install -m 0644 ${WORKDIR}/virtual_sensor_config.json ${D}${datadir}/phosphor-virtual-sensor/
}

