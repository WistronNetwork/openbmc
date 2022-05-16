SUMMARY = "OpenBMC for Wistron Network - Applications"
PR = "r1"

inherit packagegroup

PROVIDES = "${PACKAGES}"
PACKAGES = " \
        ${PN}-extras \
        ${PN}-fans \
        ${PN}-flash \
        ${PN}-system \
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

# RDEPENDS_PN_SYSTEM_EXTRAS:wistron-net-withhost =
RDEPENDS_PN_SYSTEM_EXTRAS = " \
        phosphor-sel-logger \
        ipmitool \
        phosphor-post-code-manager \
        phosphor-host-postd \
        tzdata-core \
        "

SUMMARY:${PN}-system = "Wistron Network System"
RDEPENDS:${PN}-system = " \
        entity-manager \
        dbus-sensors \
        bmcweb \
        webui-vue \
        ${RDEPENDS_PN_SYSTEM_EXTRAS} \
        "
