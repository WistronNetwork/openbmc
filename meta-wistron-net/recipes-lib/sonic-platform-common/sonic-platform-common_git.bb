SUMMARY = "Python packages which provide a common interface to platform-specific hardware peripherals in SONiC"
DESCRIPTION = "Library for accessing sonic xcvr"
SECTION = "base"
PR = "r1"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=51c38f467fdc77a6defaeffbafd318e6"

inherit setuptools3 update-alternatives

PV = "0.1+git${SRCPV}"

S = "${WORKDIR}/git"

SRCREV="285a4dd60d108e1996369d7ca4401671b571aa7e"
SRC_URI += "git://github.com/sonic-net/sonic-platform-common.git;branch=master;protocol=https \
            file://0001-Add-sonic_py_common-module.patch \
            file://0002-c_cmis.py-Use-local-sonic_py_common-module.patch \
            file://0003-Delete-unused-files-which-is-related-files-for-sonic.patch \
            file://0004-Keep-sfp-related-function-remove-others.patch \
            file://0005-Only-import-SFP-related-module.patch \
            file://0006-Only-setup-and-install-SFP-related-module.patch \
           "

BBCLASSEXTEND = "native"

DEPENDS += " \
    ${PYTHON_PN}-pytest-runner-native \
    ${PYTHON_PN}-wheel-native \
"

RDEPENDS:${PN} += " \
  ${PYTHON_PN}-distutils \
  ${PYTHON_PN}-natsort \
  ${PYTHON_PN}-terminal \
  ${PYTHON_PN}-pyyaml \
"
