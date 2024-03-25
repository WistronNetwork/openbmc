FILESEXTRAPATHS:prepend:chita := "${THISDIR}/${PN}:"

SRC_URI:append:chita = " \
    file://blacklist.json \
    file://chita_fruid.hpp \
    file://chita-baseboard.json \
    "
