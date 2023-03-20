SUMMARY = "OpenBMC Utilities from facebook/openbmc"
DESCRIPTION = "Various OpenBMC utilities"
SECTION = "utils"
LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://COPYING;md5=eb723b61539feef013de476e68b5c50a"

PV = "0.1+git${SRCPV}"

S = "${WORKDIR}/files"

RDEPENDS:${PN} += "bash"

SRCREV="c34232e791b37f292bf219782d3d04d663c03ae2"
SRC_URI += "git://github.com/facebook/openbmc;branch=helium;protocol=https;subpath=common/recipes-utils/openbmc-utils/files/ \
            file://boot-info \
            file://board-utils.sh \
            file://fan-util \
            file://mux-util \
            file://i2c-utils.sh \
           "

OPENBMC_UTILS_FILES = " \
    openbmc-utils.sh \
    shell-utils.sh \
    i2c-utils.sh \
    gpio-utils.sh \
    fan-util \
    flashrom-utils.sh \
    boot_info.sh \
    board-utils.sh \
    mux-util \
    disable_watchdog.sh \
    "

do_configure() {
    cp -rfv ${WORKDIR}/board-utils.sh ${S}
    cp -rfv ${WORKDIR}/boot-info/${SOC_FAMILY}/boot_info.sh ${S}
    cp -rfv ${WORKDIR}/fan-util ${S}
    cp -rfv ${WORKDIR}/mux-util ${S}
    cp -rfv ${WORKDIR}/i2c-utils.sh ${S}
    cp -rfv ${S}/${SOC_FAMILY}/disable_watchdog.sh ${S}

    if [ -f "${WORKDIR}/platform-board-utils.sh" ]; then
        cp -rfv ${WORKDIR}/platform-board-utils.sh ${S}
    fi
}

do_install() {
    dstdir="${D}/usr/local/packages/utils"
    localbindir="${D}/usr/local/bin"
    install -d $dstdir
    install -d $localbindir

    for f in ${OPENBMC_UTILS_FILES}; do
        install -m 755 $f ${dstdir}/${f}
        ln -snf ../packages/utils/$f ${localbindir}/$f
    done
}

PACKAGEDIR = "${prefix}/local/packages"
FILES:${PN} = "${PACKAGEDIR}/utils ${prefix}/local/bin"
