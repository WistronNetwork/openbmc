DEPENDS:append:chita = " chita-yaml-config"

CXXFLAGS:prepend = ' -DBMC_PATH="/sys/bus/i2c/devices/8-0051/eeprom"'

EXTRA_OEMESON:chita = " \
    -Dsensor-yaml-gen=${STAGING_DIR_HOST}${datadir}/chita-yaml-config/ipmi-sensors.yaml \
    -Dfru-yaml-gen=${STAGING_DIR_HOST}${datadir}/chita-yaml-config/ipmi-fru-read.yaml \
    "
