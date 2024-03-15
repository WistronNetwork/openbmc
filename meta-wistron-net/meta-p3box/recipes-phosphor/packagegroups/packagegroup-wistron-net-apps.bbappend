RDEPENDS:${PN}-system:remove:p3box = " \
        phosphor-ipmi-kcs \
        phosphor-ipmi-ipmb \
        phosphor-post-code-manager \
        phosphor-host-postd \
        "

RDEPENDS:${PN}-system:append:p3box = " \
        phosphor-virtual-sensor \
        "
