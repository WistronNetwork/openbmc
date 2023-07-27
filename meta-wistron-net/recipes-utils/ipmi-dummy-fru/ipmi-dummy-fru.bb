SUMMARY = "Create a dummy IPMI FRU when empty eeprom is detected"
PR = "r1"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

S = "${WORKDIR}"

inherit obmc-phosphor-systemd

DEPENDS = "systemd"
RDEPENDS:${PN} = "bash"

SRC_URI = "file://ipmi-dummy-fru \
           file://ipmi-dummy-fru@.service \
           file://frugen \
          "

SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE:${PN} = " ipmi-dummy-fru@.service"

do_install() {
    localbindir="${D}/usr/local/bin"
    frugendir="${D}/etc/frugen"
    install -d ${localbindir}
    install -d ${frugendir}
    install -m 755 ipmi-dummy-fru ${localbindir}/ipmi-dummy-fru

    if [ -d "${MACHINE}_frugen" ]; then
        frugen_path="${MACHINE}_frugen"
    else
        frugen_path="frugen"
    fi

    for jsonfile in $frugen_path/*.json
    do
        if [ -f "$jsonfile" ]; then
            install -m 644 $jsonfile ${frugendir}
        fi
    done

    envdir="obmc/ipmi-dummy-fru"
    for envfile in $envdir/*.conf
    do
        if [ -f "$envfile" ]; then
            env="${D}/etc/default/${envdir}"
            install -d ${env}
            install -m 644 ${envfile} ${D}/etc/default/${envfile}
        fi
    done
}

FILES:${PN} = " ${prefix}/local/bin /etc/default /etc/frugen"
