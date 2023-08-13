#!/usr/bin/env python

#############################################################################
# Sfp contains an implementation of SONiC Platform Base API and
# provides the sfp device status which are available in the platform
#############################################################################
from ctypes import create_string_buffer

try:
    from sonic_platform_base.sonic_xcvr.sfp_optoe_base import SfpOptoeBase
    from sonic_platform_base.sonic_py_common.logger import Logger
    import glob
except ImportError as e:
    raise ImportError(str(e) + "- required module not found")


# Global logger class instance
logger = Logger()

class Sfp(SfpOptoeBase):
    """Platform-specific Sfp class"""

    _port_to_i2c_mapping = {
        1:  32,
        2:  33,
        3:  34,
        4:  35,
        5:  36,
        6:  37,
        7:  38,
        8:  39,
        9:  40,
        10: 41,
        11: 42,
        12: 43,
        13: 44,
        14: 45,
        15: 46,
        16: 47,
        17: 48,
        18: 49,
        19: 50,
        20: 51,
        21: 52,
        22: 53,
        23: 54,
        24: 55,
        25: 56,
        26: 57,
        27: 58,
        28: 59,
        29: 60,
        30: 61,
        31: 62,
        32: 63,
    }

    def __init__(self, sfp_index):
        SfpOptoeBase.__init__(self)
        # Init index
        self.index = sfp_index
        self.port_num = self.index

        # Init eeprom path
        self.eeprom_path = '/sys/bus/i2c/devices/{0}-0050/eeprom'.format(self._port_to_i2c_mapping[self.port_num])
        self.PRESENT_1_16_PATH = '/sys/bus/i2c/devices/3-0011/hwmon/hwmon*/qsfpdd_p{0}_present_n'
        self.PRESENT_17_32_PATH = '/sys/bus/i2c/devices/3-0012/hwmon/hwmon*/qsfpdd_p{0}_present_n'

    def get_presence(self):
        """
        Retrieves the presence of the SFP
        Returns:
            bool: True if SFP is present, False if not

        """
        presence = False
        try:
            if self.port_num <= 16:
                pres_path=self.PRESENT_1_16_PATH.format('%02d' % self.port_num)
            else:
                pres_path=self.PRESENT_17_32_PATH.format('%02d' % self.port_num)
            for filename in glob.glob(pres_path):
                with open(filename, 'r') as sfp_presence:
                    presence = False if int(sfp_presence.read(), 0) else True
        except IOError:
            return False
        logger.log_info("debug:port_ %s sfp presence is %s" % (str(self.port_num), str(presence)))
        return presence

    def get_eeprom_path(self):
        return self.eeprom_path
