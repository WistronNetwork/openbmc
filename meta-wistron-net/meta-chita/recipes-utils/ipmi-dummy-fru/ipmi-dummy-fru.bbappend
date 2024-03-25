FILESEXTRAPATHS:prepend:chita := "${THISDIR}/${PN}:"

SRC_URI:append:chita = "file://obmc"

SYSTEMD_SERVICE:${PN}:append:chita = " \
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
