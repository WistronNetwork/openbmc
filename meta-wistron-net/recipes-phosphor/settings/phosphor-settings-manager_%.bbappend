FILESEXTRAPATHS:append:wistron-net := ":${THISDIR}/${PN}"
SRC_URI:append:wistron-net = " file://chassis_pohcounter.override.yml \
                               file://chassis_capabilities.override.yml \
                             "
