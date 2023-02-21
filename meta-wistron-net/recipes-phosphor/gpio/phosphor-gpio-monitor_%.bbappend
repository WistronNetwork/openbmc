FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRCREV = "b5bd88532ccef5e82810a1472af3d8a9213873ae"

SRC_URI:append = " file://wistron-gpio-monitor@.service"

do_configure:append() {
    cp -rfv ${WORKDIR}/wistron-gpio-monitor@.service ${S}/phosphor-gpio-monitor@.service
}
