FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = "file://platform-board-utils.sh \
                     "

OPENBMC_UTILS_FILES += " \
    platform-board-utils.sh \
    "
