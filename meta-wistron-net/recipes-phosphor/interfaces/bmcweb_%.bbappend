SRCREV = "6eaf0bdfb271c40f9553b8e10e88f79a6a45684a"

EXTRA_OEMESON:append = " \
     -Dinsecure-tftp-update=enabled \
     -Dredfish-dbus-log=enabled \
     -Dredfish-bmc-journal=enabled \
     -Dredfish-dump-log=enabled \
     -Dredfish-cpu-log=enabled \
     "
