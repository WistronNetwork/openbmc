FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRCREV = "51cff4b46543f74ffda77443f04a3ce0f16b4f27"

DEPENDS += "${PYTHON_PN}-jsonschema-native \
"

SRC_URI:append = " \
    file://0001-Add-UCD90160-device-type.patch \
    "

inherit pkgconfig meson systemd python3native
