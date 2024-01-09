FILESEXTRAPATHS:prepend:p3box := "${THISDIR}/${PN}:"

SRC_URI:append:p3box = " \
    file://config.json \
    "

FILES:${PN}:append:p3box = " ${datadir}/swampd/config.json"

do_install:append:p3box() {
    install -d ${D}${datadir}/swampd
    install -m 0644 -D ${WORKDIR}/config.json ${D}${datadir}/swampd/config.json
}
