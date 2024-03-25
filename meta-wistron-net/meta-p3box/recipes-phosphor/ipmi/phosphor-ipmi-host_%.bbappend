DEPENDS:append = " p3box-yaml-config"

CXXFLAGS:prepend = ' -DBMC_PATH="/sys/bus/i2c/devices/8-0051/eeprom"'

EXTRA_OEMESON = " \
    -Dsensor-yaml-gen=${STAGING_DIR_HOST}${datadir}/p3box-yaml-config/ipmi-sensors.yaml \
    -Dfru-yaml-gen=${STAGING_DIR_HOST}${datadir}/p3box-yaml-config/ipmi-fru-read.yaml \
    "
