SUMMARY = "Command line utilities for the SONiC project"
DESCRIPTION = "Utilities for the SONiC project"
SECTION = "base"
PR = "r1"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=51c38f467fdc77a6defaeffbafd318e6"

inherit setuptools3 update-alternatives

PV = "0.1+git${SRCPV}"

S = "${WORKDIR}/git"

SRCREV="7e24463f88ba64f485e8ae5c33e00ab3b152e073"
SRC_URI += "git://github.com/sonic-net/sonic-utilities.git;branch=master;protocol=https \
            file://0001-Remove-unused-files-which-is-related-files-for-sonic.patch \
            file://0002-Only-setup-and-install-sfputil-related-module.patch \
            file://0003-Remove-unused-functions-and-modify-sfp-related-funct.patch \
           "

BBCLASSEXTEND = "native"

DEPENDS += " \
    ${PYTHON_PN}-pytest-runner-native \
    ${PYTHON_PN}-wheel-native \
"
RDEPENDS:${PN} += " \
    ${PYTHON_PN}-click \
    ${PYTHON_PN}-natsort \
    ${PYTHON_PN}-tabulate \
    sonic-platform-common \
    sonic-platform \
"
