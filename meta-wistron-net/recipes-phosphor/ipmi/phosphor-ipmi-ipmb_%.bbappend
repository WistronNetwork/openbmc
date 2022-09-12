do_install:append() {
    ipmb_channel="${WORKDIR}/ipmb-channels.json"

    if [ -f "$ipmb_channel" ]; then
        install -m 0644 -D ${ipmb_channel} ${D}/usr/share/ipmbbridge
    fi
}
