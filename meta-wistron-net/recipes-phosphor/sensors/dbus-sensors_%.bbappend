FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRCREV = "ce6bcdfc28f60173093087050a43adbc586fd6fa"

SRC_URI:append = " \
    file://0001-PSUSensor-Add-device-type-ucd90160.patch \
    file://0002-Add-device-type-sfphwmon-to-the-allowed-list.patch \
    file://0003-PSUSensor-Add-device-type-xdpe132g5c-and-xdpe132g5d.patch \
    file://0004-add-device-type-tmp432-and-isl68239.patch \
    file://0005-PSUSensor-Add-device-type-pmbus_psu.patch \
    file://0006-add-gpio-type-to-handle-cpu-power-status.patch \
    file://0007-PSUSensor-Add-device-type-powr1220.patch \
    file://0008-PSUSensor-Add-device-type-optoe.patch \
    file://0009-PSUSensor-Add-vmon1-to-vmon12-for-powr1220-driver.patch \
    file://0010-Add-ec-type-and-elevenBitScale3-translatation.patch \
    "
