DISTROOVERRIDES .= ":wistron-net"

MACHINE_FEATURES += "\
        obmc-bmc-state-mgmt \
        obmc-phosphor-fan-mgmt \
        obmc-phosphor-flash-mgmt \
        obmc-chassis-state-mgmt \
        obmc-host-ipmi \
        obmc-host-state-mgmt \
        obmc-phosphor-chassis-mgmt \
        "

VIRTUAL-RUNTIME_skeleton_workbook = "${MACHINE}-config"

PREFERRED_PROVIDER_virtual/obmc-chassis-mgmt = "packagegroup-wistron-net-apps"
PREFERRED_PROVIDER_virtual/obmc-fan-mgmt = "packagegroup-wistron-net-apps"
PREFERRED_PROVIDER_virtual/obmc-flash-mgmt = "packagegroup-wistron-net-apps"
PREFERRED_PROVIDER_virtual/obmc-inventory-data ?= "${VIRTUAL-RUNTIME_skeleton_workbook}"
PREFERRED_PROVIDER_virtual/obmc-system-mgmt = "packagegroup-wistron-net-apps"
PREFERRED_PROVIDER_virtual/obmc-host-ipmi-hw ?= "phosphor-ipmi-kcs"

VIRTUAL-RUNTIME_obmc-host-state-manager ?= "x86-power-control"
VIRTUAL-RUNTIME_obmc-chassis-state-manager ?= "x86-power-control"

PREFERRED_VERSION_boost = "1.78.0"

# It has no ikvm feature for our network device.
IMAGE_FEATURES:remove = "obmc-ikvm"

# Most of our hardware uses the default ttyS4 at 115200.
# If platform has difference setting, it can be changed in <meta-platform>/conf/machine/<platform>.conf
SERIAL_CONSOLES:wistron-net = "115200;ttyS4"
