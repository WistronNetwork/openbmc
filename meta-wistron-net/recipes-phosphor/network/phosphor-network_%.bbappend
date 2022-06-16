FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

DEPENDS += "nlohmann-json boost"

SRC_URI += " file://0003-Adding-channel-specific-privilege-to-network.patch \
           "

EXTRA_OECONF:append = " --enable-nic-ethtool=yes"
EXTRA_OECONF:append = " --enable-ipv6-accept-ra=yes"
