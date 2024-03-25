FILESEXTRAPATHS:prepend := "${THISDIR}/u-boot-aspeed-sdk:"

SRC_URI += "file://p3box.cfg \
            file://p3box.dts \
           "

do_compile:prepend () {
  cp -rfv ${WORKDIR}/p3box.dts ${S}/arch/arm/dts/ast2500-evb.dts
}
