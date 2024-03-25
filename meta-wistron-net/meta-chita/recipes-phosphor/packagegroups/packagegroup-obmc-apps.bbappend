RDEPENDS:${PN}-extras:remove:chita = " libpeci \
                                       iperf3 \
                                       jbi \
                                       opkg \
                                       phosphor-misc-usb-ctrl \
                                       tcpdump \
                                       flashrom \
                                     "

RDEPENDS:${PN}-console:remove:chita = "obmc-console"

RDEPENDS:${PN}-bmcweb:remove:chita = "bmcweb"

RDEPENDS:${PN}-sensors:remove:chita = " \
        ${VIRTUAL-RUNTIME_obmc-sensors-hwmon} \
        "

RDEPENDS:${PN}-extras:append:chita = " packagegroup-obmc-python3 \
                                     "
