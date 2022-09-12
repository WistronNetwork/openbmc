FILESEXTRAPATHS:append:= "${THISDIR}/files:"

SRC_URI:append: = " \
    file://0001-aspeed-BOOTARGS-use-mem.devmem-1.patch \
    file://0001-drivers-mtd-spi-Support-W25Q512JV.patch \
    "
