FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRCREV = "ce6bcdfc28f60173093087050a43adbc586fd6fa"

SRC_URI:append = " \
    file://0001-PSUSensor-Add-device-type-ucd90160.patch \
    file://0002-Add-device-type-sfphwmon-to-the-allowed-list.patch \
    file://0003-PSUSensor-Add-device-type-xdpe132g5c-and-xdpe132g5d.patch \
    file://0004-add-device-type-tmp432-and-isl68239.patch \
    file://0005-PSUSensor-Add-device-type-pmbus_psu.patch \
    "
