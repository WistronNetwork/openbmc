FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRCREV = "628058fadb9d4c35dbe2c9636f3e6e93767e9526"

SRC_URI:append = " \
    file://0001-sel_logger-Use-d-bus-message-and-rsyslog-for-sel.patch \
    "

PACKAGECONFIG:append = " log-threshold log-pulse send-to-logger log-alarm clears-sel"
