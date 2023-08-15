FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = " \
    file://0001-Disable-some-item-which-we-don-t-support.patch \
    "
