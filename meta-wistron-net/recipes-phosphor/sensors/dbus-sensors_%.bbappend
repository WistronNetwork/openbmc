FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRCREV = "ce6bcdfc28f60173093087050a43adbc586fd6fa"

SRC_URI:append = " \
    file://0001-PSUSensor-Add-device-type-ucd90160.patch \
    "
