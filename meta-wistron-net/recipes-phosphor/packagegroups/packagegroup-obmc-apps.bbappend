RDEPENDS:${PN}-extras:append += " bmc-mac-setup \
                                  cpldupdate \
                                  cpldupdate-i2c \
                                  cpldupdate-jtag \
                                  ethtool \
                                  flashrom \
                                  frugen \
                                  gpiocli \
                                  iperf3 \
                                  ipmitool \
                                  ipmi-dummy-fru \
                                  ipmi-fru-parser \
                                  jbi \
                                  jq \
                                  libpeci \
                                  libcpldupdate-dll-gpio \
                                  libcpldupdate-dll-ioctl \
                                  lmsensors-sensors \
                                  memtester \
                                  openbmc-utils \
                                  opkg \
                                  phosphor-misc-usb-ctrl \
                                  phytool \
                                  pv \
                                  tcpdump \
                                  wdtcli \
                                "

RDEPENDS:${PN}-sensors:remove = " ${VIRTUAL-RUNTIME_obmc-sensors-hwmon} \
                                "

RDEPENDS:${PN}-inventory:remove = " ${VIRTUAL-RUNTIME_obmc-fan-presence} \
                                  "

RDEPENDS:${PN}-fan-control:remove = " ${VIRTUAL-RUNTIME_obmc-fan-control} \
                                      phosphor-fan-monitor \
                                    "
