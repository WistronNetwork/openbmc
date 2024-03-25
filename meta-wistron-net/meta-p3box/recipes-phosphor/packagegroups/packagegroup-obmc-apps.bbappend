RDEPENDS:${PN}-extras:remove:p3box = " libpeci \
                                       iperf3 \
                                       jbi \
                                       opkg \
                                       phosphor-misc-usb-ctrl \
                                       tcpdump \
                                       flashrom \
                                     "

RDEPENDS:${PN}-console:remove:p3box = "obmc-console"

RDEPENDS:${PN}-bmcweb:remove:p3box = "bmcweb"

RDEPENDS:${PN}-sensors:remove:p3box = " \
        ${VIRTUAL-RUNTIME_obmc-sensors-hwmon} \
        "

RDEPENDS:${PN}-extras:append:p3box = " packagegroup-obmc-python3 \
                                     "
