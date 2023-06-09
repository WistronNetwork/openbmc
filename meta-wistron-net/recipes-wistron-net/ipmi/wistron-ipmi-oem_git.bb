SUMMARY = "Wistron net OEM IPMI commands"
DESCRIPTION = "Wistron net OEM IPMI commands"

LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

PV = "0.1+git${SRCPV}"

S = "${WORKDIR}/git"

DEPENDS = "boost systemd phosphor-ipmi-host phosphor-logging"
DEPENDS += "libgpiod libobmc-i2c libgpio-ctrl libpsu libxcvr"
DEPENDS += "${PYTHON_PN}-pyyaml-native ${PYTHON_PN}-mako-native"

inherit meson pkgconfig obmc-phosphor-ipmiprovider-symlink phosphor-ipmi-host

PACKAGECONFIG ??= ""
PACKAGECONFIG[oem-platform] = "-Doem-platform=enabled,-Doem-platform=disabled"

LIBRARY_NAMES = "libzwistronoemcmds.so"

SRC_URI = "git://git@10.31.80.71/justine_team/openbmc/wistron-net-ipmi-oem.git;branch=master;protocol=ssh"
SRCREV = "7eff2757a7a897d8d8a02e72439a9e27fb7914a0"

sensor_yaml_path = "${STAGING_DIR_NATIVE}/../recipe-sysroot/usr/share/${MACHINE}-yaml-config"

EXTRA_OEMESON = " \
        -Dsensor-yaml-gen=${sensor_yaml_path}/ipmi-sensors.yaml \
        "

HOSTIPMI_PROVIDER_LIBRARY += "${LIBRARY_NAMES}"
NETIPMI_PROVIDER_LIBRARY += "${LIBRARY_NAMES}"

FILES:${PN}:append = " ${libdir}/ipmid-providers/lib*${SOLIBS}"
FILES:${PN}:append = " ${libdir}/host-ipmid/lib*${SOLIBS}"
FILES:${PN}:append = " ${libdir}/net-ipmid/lib*${SOLIBS}"
FILES:${PN}-dev:append = " ${libdir}/ipmid-providers/lib*${SOLIBSDEV}"

do_configure:prepend() {
    fru_file="${S}/platform/include/fru/${MACHINE}_fru.hpp"

    if [ -f "$fru_file" ]; then
        cp -rfv ${fru_file} ${S}/include/fru.hpp
    fi

    oem_platform_h_file="${S}/platform/include/oem_lib/${MACHINE}_platform.hpp"

    if [ -f "$oem_platform_h_file" ]; then
        cp -rfv ${oem_platform_h_file} ${S}/include/oem_platform.hpp
    fi

    oem_platform_c_file="${S}/platform/src/oem_lib/${MACHINE}_platform.cpp"

    if [ -f "$oem_platform_c_file" ] &&
       [[ "${PACKAGECONFIG}" == *oem-platform* ]]; then
        cp -rfv ${oem_platform_c_file} ${S}/src/oem_platform.cpp
    fi

    platform_psu_h_file="${S}/platform/include/psu/${MACHINE}_psu-info.hpp"

    if [ -f "$platform_psu_h_file" ]; then
        cp -rfv ${platform_psu_h_file} ${S}/include/psu-info.hpp
    fi

    platform_psu_c_file="${S}/platform/src/psu/${MACHINE}_psu-info.cpp"

    if [ -f "$platform_psu_c_file" ]; then
        cp -rfv ${platform_psu_c_file} ${S}/src/psu-info.cpp
    fi

    platform_fan_h_file="${S}/platform/include/fan/${MACHINE}_fan-info.hpp"

    if [ -f "$platform_fan_h_file" ]; then
        cp -rfv ${platform_fan_h_file} ${S}/include/fan-info.hpp
    fi

    platform_fan_c_file="${S}/platform/src/fan/${MACHINE}_fan-info.cpp"

    if [ -f "$platform_fan_c_file" ]; then
        cp -rfv ${platform_fan_c_file} ${S}/src/fan-info.cpp
    fi

    platform_firmware_c_file="${S}/platform/src/firmware/${MACHINE}_firmware-info.cpp"

    if [ -f "$platform_firmware_c_file" ]; then
        cp -rfv ${platform_firmware_c_file} ${S}/src/firmware-info.cpp
    fi

    platform_led_h_file="${S}/platform/include/led/${MACHINE}_led-info.hpp"

    if [ -f "$platform_led_h_file" ]; then
        cp -rfv ${platform_led_h_file} ${S}/include/led-info.hpp
    fi

    platform_led_c_file="${S}/platform/src/led/${MACHINE}_led-info.cpp"

    if [ -f "$platform_led_c_file" ]; then
        cp -rfv ${platform_led_c_file} ${S}/src/led-info.cpp
    fi

    platform_xcvr_h_file="${S}/platform/include/xcvr/${MACHINE}_xcvr-info.hpp"

    if [ -f "$platform_xcvr_h_file" ]; then
        cp -rfv ${platform_xcvr_h_file} ${S}/include/xcvr-info.hpp
    fi

    platform_xcvr_c_file="${S}/platform/src/xcvr/${MACHINE}_xcvr-info.cpp"

    if [ -f "$platform_xcvr_c_file" ]; then
        cp -rfv ${platform_xcvr_c_file} ${S}/src/xcvr-info.cpp
    fi

    default_sensor_yaml_file="${S}/scripts/ipmi-sensors.yaml"

    if [ ! -f "${sensor_yaml_path}/ipmi-sensors.yaml" ]; then
        mkdir -p ${sensor_yaml_path}
        cp -rfv ${default_sensor_yaml_file} ${sensor_yaml_path}
    fi
}

do_install:append() {
    install -d ${D}${includedir}/wistron-ipmi-oem
    install -m 0644 -D ${S}/include/*.hpp ${D}${includedir}/wistron-ipmi-oem
}
