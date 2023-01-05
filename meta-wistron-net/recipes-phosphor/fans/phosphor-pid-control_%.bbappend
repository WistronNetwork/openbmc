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
