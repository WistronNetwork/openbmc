FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = " \
    file://blacklist.json \
    file://p3box_fruid.hpp \
    file://p3box-baseboard.json \
    "
