SUMMARY = "Python packages which provide platform-specific hardware peripherals"
DESCRIPTION = "Library for platform Layer"
SECTION = "base"
PR = "r1"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=51c38f467fdc77a6defaeffbafd318e6"

inherit setuptools3 update-alternatives

S = "${WORKDIR}"

SRC_URI += "file://LICENSE \
            file://sonic_platform \
            file://setup.py \
           "

BBCLASSEXTEND = "native"
