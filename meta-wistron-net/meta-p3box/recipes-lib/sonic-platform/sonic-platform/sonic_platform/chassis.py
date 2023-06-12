#!/usr/bin/env python

#############################################################################
#
# Module contains an implementation of SONiC Platform Base API and
# provides the Chassis information which are available in the platform
#
#############################################################################
try:
    import sys
    from sonic_platform_base.chassis_base import ChassisBase
except ImportError as e:
    raise ImportError(str(e) + "- required module not found")

NUM_SFP = 32

class Chassis(ChassisBase):
    """Platform-specific Chassis class"""

    def __init__(self):
        ChassisBase.__init__(self)
        self.__initialize_sfp()

    def __initialize_sfp(self):
        from sonic_platform.sfp import Sfp
        for index in range(1, NUM_SFP + 1):
            sfp_module = Sfp(index)
            self._sfp_list.append(sfp_module)

    def get_sfp(self, index):
        sfp = None
        try:
            if index < 1:
                sys.stderr.write("SFP port {} out of range (1-{})\n".format(index, len(self._sfp_list)))
                return sfp

            sfp = self._sfp_list[index - 1]
        except IndexError:
            sys.stderr.write("SFP port {} out of range (1-{})\n".format(index, len(self._sfp_list)))

        return sfp
