FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRCREV = "f6e4c1f588558e5bf8d639349c09268f3511f42a"

DEPENDS += "${PYTHON_PN}-jsonschema-native \
"

SRC_URI:append = " \
    file://0001-Add-UCD90160-device-type.patch \
    file://0002-Add-Fruid-Item.patch \
    file://0003-Add-baseoffset-to-process-bmc-fru.patch \
    file://0004-Add-SFPHWMON-device-type.patch \
    file://0005-Fix-build-error-when-ENABLE_FRU_AREA_RESIZE.patch \
    file://fruid.hpp \
    "

inherit python3native

PACKAGECONFIG[fru-device-resizefru] = "-Dfru-device-resizefru=true, -Dfru-device-resizefru=false"
PACKAGECONFIG:append = " fru-device-resizefru"

do_compile:prepend() {
    fruid_file="${WORKDIR}/${MACHINE}_fruid.hpp"

    if [ -f "$fruid_file" ]; then
        cp -rfv ${fruid_file} ${S}/include/fruid.hpp
    else
        cp -rfv ${WORKDIR}/fruid.hpp ${S}/include/fruid.hpp
    fi
}
