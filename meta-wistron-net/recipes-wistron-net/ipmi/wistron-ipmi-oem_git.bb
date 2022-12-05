SUMMARY = "Wistron net OEM IPMI commands"
DESCRIPTION = "Wistron net OEM IPMI commands"

LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

PV = "0.1+git${SRCPV}"

S = "${WORKDIR}/git"

DEPENDS = "boost phosphor-ipmi-host phosphor-logging systemd libgpiod libobmc-i2c libgpio-ctrl"
DEPENDS += "${PYTHON_PN}-pyyaml-native ${PYTHON_PN}-mako-native"

inherit meson pkgconfig obmc-phosphor-ipmiprovider-symlink phosphor-ipmi-host

PACKAGECONFIG ??= ""
PACKAGECONFIG[platform-oem] = "-Dplatform-oem=enabled,-Dplatform-oem=disabled"

LIBRARY_NAMES = "libzwistronoemcmds.so"

SRC_URI = "git://git@10.31.80.71/justine_team/openbmc/wistron-net-ipmi-oem.git;branch=master;protocol=ssh"
SRCREV = "42ea5bb6cf1379eb187473c4ce7703ab6a6472c4"

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

    platform_file="${S}/platform/include/oemcommands/${MACHINE}_platform.hpp"

    if [ -f "$platform_file" ]; then
        cp -rfv ${platform_file} ${S}/include/oem_platform.hpp
    fi

    platform_oem_file="${S}/platform/src/${MACHINE}_oemcommands.cpp"

    if [ -f "$platform_oem_file" ] &&
       [[ "${PACKAGECONFIG}" == *platform-oem* ]]; then
        cp -rfv ${platform_oem_file} ${S}/src/platform_oemcommands.cpp
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
