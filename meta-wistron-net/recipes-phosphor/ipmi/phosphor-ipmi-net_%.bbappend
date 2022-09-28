FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = " \
    file://0001-Support-more-algorithm-for-more-chipher-suites.patch \
    "
