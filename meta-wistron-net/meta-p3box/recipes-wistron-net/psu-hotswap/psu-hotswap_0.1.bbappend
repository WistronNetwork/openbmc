FILESEXTRAPATHS:prepend:p3box := "${THISDIR}/${PN}:"

SRC_URI:append:p3box = " file://psu-hotswap"

PSU_INSTANCES = "1 2"
