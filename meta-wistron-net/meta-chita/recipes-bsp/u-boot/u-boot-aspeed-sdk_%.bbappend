FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://chita.cfg \
            file://chita.dts \
           "

do_compile:prepend () {
  cp -rfv ${WORKDIR}/chita.dts ${S}/arch/arm/dts/ast2500-evb.dts
}
