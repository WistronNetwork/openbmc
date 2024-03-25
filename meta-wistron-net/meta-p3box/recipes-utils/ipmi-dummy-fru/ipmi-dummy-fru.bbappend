FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = "file://obmc"

SYSTEMD_SERVICE:${PN}:append = " \
    ${PN}@bmc.service \
    ${PN}@mb.service \
    ${PN}@fan1.service \
    ${PN}@fan2.service \
    ${PN}@fan3.service \
    ${PN}@fan4.service \
    ${PN}@fan5.service \
    ${PN}@fan6.service \
    ${PN}@psu1.service \
    ${PN}@psu2.service \
    "
