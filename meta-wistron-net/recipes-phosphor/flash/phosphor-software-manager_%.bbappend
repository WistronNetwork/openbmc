SRCREV = "cce26eb063488a4fe3403e55979cac7f8f6a7547"

SOFTWARE_MGR_PACKAGES += " \
    ${PN}-usb \
    ${PN}-side-switch \
"

PACKAGECONFIG:append = " static-dual-image"
PACKAGECONFIG[usb_code_update] = "-Dusb-code-update=enabled, -Dusb-code-update=disabled, cli11"
PACKAGECONFIG[side_switch_on_boot] = "-Dside-switch-on-boot=enabled, -Dside-switch-on-boot=disabled, cli11"
PACKAGECONFIG[static-dual-image] = "-Dbmc-static-dual-image=enabled, -Dbmc-static-dual-image=disabled"

# The repo installs several scripts that depends on bash
RDEPENDS:${PN} += " bash"

FILES:${PN}-usb += "\
    ${base_libdir}/udev/rules.d/70-bmc-usb.rules \
    ${bindir}/phosphor-usb-code-update \
    "

FILES:${PN}-side-switch += "\
    ${bindir}/phosphor-bmc-side-switch \
    "

SYSTEMD_SERVICE:${PN}-usb += "${@bb.utils.contains('PACKAGECONFIG', 'usb_code_update', 'usb-code-update@.service', '', d)}"
SYSTEMD_SERVICE:${PN}-side-switch += "${@bb.utils.contains('PACKAGECONFIG', 'side_switch_on_boot', 'phosphor-bmc-side-switch.service', '', d)}"
SYSTEMD_SERVICE:${PN}-updater += "${@bb.utils.contains('PACKAGECONFIG', 'static-dual-image', 'obmc-flash-bmc-alt@.service', '', d)}"
SYSTEMD_SERVICE:${PN}-updater += "${@bb.utils.contains('PACKAGECONFIG', 'static-dual-image', 'obmc-flash-bmc-static-mount-alt.service', '', d)}"
SYSTEMD_SERVICE:${PN}-updater += "${@bb.utils.contains('PACKAGECONFIG', 'static-dual-image', 'obmc-flash-bmc-prepare-for-sync.service', '', d)}"
