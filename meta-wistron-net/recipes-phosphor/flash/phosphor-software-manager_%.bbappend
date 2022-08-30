SRCREV = "35aa9a889fda41d75a35633faba1b081c70ce86c"

SOFTWARE_MGR_PACKAGES += " \
    ${PN}-usb \
"

PACKAGECONFIG:append = " static-dual-image"
PACKAGECONFIG[usb_code_update] = "-Dusb-code-update=enabled, -Dusb-code-update=disabled, cli11"
PACKAGECONFIG[static-dual-image] = "-Dbmc-static-dual-image=enabled, -Dbmc-static-dual-image=disabled"

# The repo installs several scripts that depends on bash
RDEPENDS:${PN} += " bash"

FILES:${PN}-usb += "\
    ${base_libdir}/udev/rules.d/70-bmc-usb.rules \
    ${bindir}/phosphor-usb-code-update \
    "

SYSTEMD_SERVICE:${PN}-usb += "${@bb.utils.contains('PACKAGECONFIG', 'usb_code_update', 'usb-code-update@.service', '', d)}"
SYSTEMD_SERVICE:${PN}-updater += "${@bb.utils.contains('PACKAGECONFIG', 'static-dual-image', 'obmc-flash-bmc-alt@.service', '', d)}"
SYSTEMD_SERVICE:${PN}-updater += "${@bb.utils.contains('PACKAGECONFIG', 'static-dual-image', 'obmc-flash-bmc-static-mount-alt.service', '', d)}"
SYSTEMD_SERVICE:${PN}-updater += "${@bb.utils.contains('PACKAGECONFIG', 'static-dual-image', 'obmc-flash-bmc-prepare-for-sync.service', '', d)}"
