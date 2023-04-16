FILESEXTRAPATHS:append:= "${THISDIR}/files:"

OVERRIDES .= ":uboot-flash-${FLASH_SIZE}"

LICENSE = "GPL-2.0-or-later"

SRCREV = "b0dc9ccd3d21315d69a526fcdb211d995a75d703"

SRC_URI += "file://fw_env_flash_nor.config \
            file://common.cfg \
           "
SRC_URI:append:uboot-flash-32768 = " file://u-boot_flash_32M.cfg"
SRC_URI:append:uboot-flash-65536 = " file://u-boot_flash_64M.cfg"

ENV_CONFIG_FILE = "fw_env_ast2600_nor"

SOCSEC_SIGN_EXTRA_OPTS += "--rsa_key_order=big"

OTPTOOL_USER_DIR ?= "${WORKDIR}/user"

inherit otptool
