FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = " \
    file://0001-Change-meson-version-0.57.0.patch \
    "

inherit meson

SRCREV = "239aa7d705e69d37383df37f6cbb67c0d9425423"

EXTRA_OEMESON = " \
  -Dtests=disabled \
  -Dsystemd_target="multi-user.target" \
  "
RDEPENDS:${PN} += "bash"

do_install:append:wistron-net() {
    fan_type_check_file="${WORKDIR}/fan-type-check"
    if [ -f "$fan_type_check_file" ]; then
        install -d ${D}/usr/libexec/phosphor-pid-control
        install -m 0777 ${fan_type_check_file} ${D}/usr/libexec/phosphor-pid-control/
    fi
}
