SUMMARY = "OpenBMC for Wistron Network - Applications"
PR = "r1"

inherit packagegroup

PROVIDES = "${PACKAGES}"
PACKAGES = " \
        ${PN}-fans \
        ${PN}-flash \
        ${PN}-system \
        ${PN}-chassis \
        ${PN}-hostmgmt \
        "

PROVIDES += "virtual/obmc-chassis-mgmt"
PROVIDES += "virtual/obmc-fan-mgmt"
PROVIDES += "virtual/obmc-flash-mgmt"
PROVIDES += "virtual/obmc-system-mgmt"

RPROVIDES:${PN}-chassis += "virtual-obmc-chassis-mgmt"
RPROVIDES:${PN}-fans += "virtual-obmc-fan-mgmt"
RPROVIDES:${PN}-flash += "virtual-obmc-flash-mgmt"
RPROVIDES:${PN}-system += "virtual-obmc-system-mgmt"

SUMMARY:${PN}-chassis = "Wistron Network Chassis"
RDEPENDS:${PN}-chassis = " \
        x86-power-control \
       "

SUMMARY:${PN}-fans = "Wistron Network Fans"
RDEPENDS:${PN}-fans = ""

SUMMARY:${PN}-flash = "Wistron Network Flash"
RDEPENDS:${PN}-flash = " \
        phosphor-software-manager \
        obmc-control-bmc \
        "

SUMMARY:${PN}-system = "Wistron Network System"
RDEPENDS:${PN}-system = " \
        bmcweb \
        webui-vue \
        entity-manager \
        dbus-sensors \
        phosphor-sel-logger \
        phosphor-ipmi-kcs \
        phosphor-ipmi-ipmb \
        phosphor-post-code-manager \
        phosphor-host-postd \
        tzdata-core \
        wistron-ipmi-oem \
        "
