SUMMARY = "NSS and PAM module for using LDAP as a naming service"
DESCRIPTION = "\
 daemon for NSS and PAM lookups using LDAP \
 This package provides a daemon for retrieving user accounts and similar \
 system information from LDAP. It is used by the libnss-ldapd and \
 libpam-ldapd packages but is not very useful by itself. \
 "

HOMEPAGE = "https://github.com/arthurdejong/nss-pam-ldapd"
S = "${WORKDIR}/git"

LICENSE = "LGPLv2.1"
LIC_FILES_CHKSUM = "file://COPYING;md5=fbc093901857fcd118f065f900982c24"
SECTION = "base"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI = "\
	git://github.com/arthurdejong/nss-pam-ldapd;branch=master;protocol=https \
	file://nslcd.init \
	file://nslcd.service \
	"

inherit autotools

SRCREV="47fd03bc80d470de881c025ff934325bd7def0b5"

DEPENDS += "libpam openldap krb5"

RDEPENDS:${PN} += "nscd"

EXTRA_OECONF = "\
	--disable-pynslcd \
	--libdir=${base_libdir} \
	--with-pam-seclib-dir=${base_libdir}/security \
	"

CONFFILES:${PN} += "${sysconfdir}/nslcd.conf"

FILES:${PN} += "${base_libdir}/security ${datadir}"
FILES:${PN}-dbg += "${base_libdir}/security/.debug"

LDAP_DN ?= "dc=my-domain,dc=com"

do_install:append() {
	install -D -m 0755 ${WORKDIR}/nslcd.init ${D}${sysconfdir}/init.d/nslcd

	sed -i -e 's/^uid nslcd/# uid nslcd/;' ${D}${sysconfdir}/nslcd.conf
	sed -i -e 's/^gid nslcd/# gid nslcd/;' ${D}${sysconfdir}/nslcd.conf
	sed -i -e 's/^base dc=example,dc=com/base ${LDAP_DN}/;' ${D}${sysconfdir}/nslcd.conf

	install -d ${D}${systemd_system_unitdir}
	install -m 0644 ${WORKDIR}/nslcd.service ${D}${systemd_system_unitdir}
}

inherit update-rc.d systemd

INITSCRIPT_NAME = "nslcd"
INITSCRIPT_PARAMS = "defaults"

SYSTEMD_SERVICE:${PN} = "nslcd.service"
