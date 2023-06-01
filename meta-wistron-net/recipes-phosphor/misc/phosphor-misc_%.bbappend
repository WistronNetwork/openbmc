SRCREV = "37c76da7c277f3531f35ec726ac690e53ab4915a"

PACKAGES = "${PHOSPHOR_MISC_PACKAGES}"

RDEPENDS:${PN}-first-boot-set-hostname += "bash"
RDEPENDS:${PN}-first-boot-set-mac += "bash"
RDEPENDS:${PN}-usb-ctrl += "bash"
