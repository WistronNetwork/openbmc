FILESEXTRAPATHS:prepend:p3box := "${THISDIR}/${PN}:"

SRC_URI:append:p3box = " file://fan-hotswap"

FAN_INSTANCES = "1 2 3 4 5 6"
