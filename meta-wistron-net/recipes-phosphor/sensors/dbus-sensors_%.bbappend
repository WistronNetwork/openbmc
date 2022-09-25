FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRCREV = "ce6bcdfc28f60173093087050a43adbc586fd6fa"

SRC_URI:append = " \
    file://0001-PSUSensor-Add-device-type-ucd90160.patch \
    file://0001-Add-device-type-sfphwmon-to-the-allowed-list.patch \
    file://0001-PSUSensor-Add-device-type-xdpe132g5c-and-xdpe132g5d.patch \
    "
