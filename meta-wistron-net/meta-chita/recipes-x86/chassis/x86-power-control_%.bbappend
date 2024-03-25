
FILESEXTRAPATHS:prepend:chita := "${THISDIR}/${PN}:"

SRC_URI:append:chita = " \
	file://power-config-host0.json \
	"

do_install:append:chita() {
	install -d  ${D}/${datadir}/${PN}
	install -m 0644 ${WORKDIR}/power-config-host0.json ${D}/${datadir}/${PN}
}
