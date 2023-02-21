FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
SRCREV = "4c04af1092314aa716b229f41831a44aee59014d"

SRC_URI:append = " \
	file://0001-modify-power-control-restart-time.patch \
	"
