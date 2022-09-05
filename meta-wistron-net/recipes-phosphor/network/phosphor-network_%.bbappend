FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRCREV = "c922d5e8f5baed49b605b378b45a226c0d657286"

DEPENDS += "nlohmann-json boost"

SRC_URI += " file://0003-Adding-channel-specific-privilege-to-network.patch \
           "

PACKAGECONFIG:append = " nic-ethtool"
PACKAGECONFIG[hyp-nw-config] = "-Dhyp-nw-config=true, -Dhyp-nw-config=false,,"

SYSTEMD_SERVICE:${PN} += "${@bb.utils.contains('PACKAGECONFIG', 'hyp-nw-config', 'xyz.openbmc_project.Network.Hypervisor.service', '', d)}"
