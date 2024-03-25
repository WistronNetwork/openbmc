FILESEXTRAPATHS:prepend:chita := "${THISDIR}/${PN}:"

SRC_URI:append:chita = "file://platform-board-utils.sh \
                     "

OPENBMC_UTILS_FILES += " \
    platform-board-utils.sh \
    "
