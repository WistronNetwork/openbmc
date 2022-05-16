DISTROOVERRIDES .= ":wistron-net"

MACHINE_FEATURES += "\
        obmc-bmc-state-mgmt \
        obmc-phosphor-fan-mgmt \
        obmc-phosphor-flash-mgmt \
        "

VIRTUAL-RUNTIME_skeleton_workbook = "${MACHINE}-config"

PREFERRED_PROVIDER_virtual/obmc-chassis-mgmt = "packagegroup-wistron-net-apps"
PREFERRED_PROVIDER_virtual/obmc-fan-mgmt = "packagegroup-wistron-net-apps"
PREFERRED_PROVIDER_virtual/obmc-flash-mgmt = "packagegroup-wistron-net-apps"
PREFERRED_PROVIDER_virtual/obmc-inventory-data ?= "${VIRTUAL-RUNTIME_skeleton_workbook}"
PREFERRED_PROVIDER_virtual/obmc-system-mgmt = "packagegroup-wistron-net-apps"

# It has no ikvm feature for our network device.
IMAGE_FEATURES:remove = "obmc-ikvm"

# Most of our hardware uses the default ttyS4 at 115200.
# If platform has difference setting, it can be changed in <meta-platform>/conf/machine/<platform>.conf
SERIAL_CONSOLES:wistron-net = "115200;ttyS4"
