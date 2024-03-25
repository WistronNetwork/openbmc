FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRCREV = "51cff4b46543f74ffda77443f04a3ce0f16b4f27"

DEPENDS += "${PYTHON_PN}-jsonschema-native \
"

SRC_URI:append = " \
    file://0001-Add-UCD90160-device-type.patch \
    file://0002-Add-Fruid-Item.patch \
    file://fruid.hpp \
    "

inherit pkgconfig meson systemd python3native

do_compile:prepend() {
    fruid_file="${WORKDIR}/${MACHINE}_fruid.hpp"

    if [ -f "$fruid_file" ]; then
        cp -rfv ${fruid_file} ${S}/include/fruid.hpp
    else
        cp -rfv ${WORKDIR}/fruid.hpp ${S}/include/fruid.hpp
    fi
}
