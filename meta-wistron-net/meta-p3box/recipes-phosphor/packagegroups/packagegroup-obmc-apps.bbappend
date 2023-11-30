RDEPENDS:${PN}-extras:remove:p3box = " libpeci \
                                       iperf3 \
                                       jbi \
                                       phosphor-misc-usb-ctrl \
                                       tcpdump \
                                       flashrom \
                                     "

RDEPENDS:${PN}-console:remove:p3box = "obmc-console"

RDEPENDS:${PN}-extras:append:p3box = " packagegroup-obmc-python3 \
                                       sonic-utilities \
                                       psu-hotswap \
                                     "
