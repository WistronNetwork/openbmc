SRCREV = "69320d54efe41fa5b0947adab5702d9023cc1ee3"

EXTRA_OEMESON:append = " \
     -Dinsecure-tftp-update=enabled \
     -Dredfish-dbus-log=enabled \
     -Dredfish-bmc-journal=enabled \
     "
