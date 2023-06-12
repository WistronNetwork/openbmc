#!/usr/bin/env python3
#
# main.py
#
# Command-line utility for interacting with SFP transceivers within SONiC
#

import os
import sys
import ast
import time
import datetime

import click
import sonic_platform
from natsort import natsorted
from sonic_platform_base.sonic_py_common import logger
from utilities_common.sfp_helper import covert_application_advertisement_to_output_string
from utilities_common.sfp_helper import QSFP_DATA_MAP
from tabulate import tabulate

VERSION = '3.0'

SYSLOG_IDENTIFIER = "sfputil"

EXIT_FAIL = -1
EXIT_SUCCESS = 0
ERROR_PERMISSIONS = 1
ERROR_CHASSIS_LOAD = 2
ERROR_SFPUTILHELPER_LOAD = 3
ERROR_NOT_IMPLEMENTED = 5
ERROR_INVALID_PORT = 6
SMBUS_BLOCK_WRITE_SIZE = 32
# Default host password as per CMIS spec:
# http://www.qsfp-dd.com/wp-content/uploads/2021/05/CMIS5p0.pdf
CDB_DEFAULT_HOST_PASSWORD = 0x00001011

MAX_LPL_FIRMWARE_BLOCK_SIZE = 116 #Bytes

PAGE_SIZE = 128
PAGE_OFFSET = 128

SFF8472_A0_SIZE = 256

# TODO: We should share these maps and the formatting functions between sfputil and sfpshow
QSFP_DD_DATA_MAP = {
    'model': 'Vendor PN',
    'vendor_oui': 'Vendor OUI',
    'vendor_date': 'Vendor Date Code(YYYY-MM-DD Lot)',
    'manufacturer': 'Vendor Name',
    'vendor_rev': 'Vendor Rev',
    'serial': 'Vendor SN',
    'type': 'Identifier',
    'ext_identifier': 'Extended Identifier',
    'ext_rateselect_compliance': 'Extended RateSelect Compliance',
    'cable_length': 'cable_length',
    'cable_type': 'Length',
    'nominal_bit_rate': 'Nominal Bit Rate(100Mbs)',
    'specification_compliance': 'Specification compliance',
    'encoding': 'Encoding',
    'connector': 'Connector',
    'application_advertisement': 'Application Advertisement',
    'active_firmware': 'Active Firmware Version',
    'inactive_firmware': 'Inactive Firmware Version',
    'hardware_rev': 'Hardware Revision',
    'media_interface_code': 'Media Interface Code',
    'host_electrical_interface': 'Host Electrical Interface',
    'host_lane_count': 'Host Lane Count',
    'media_lane_count': 'Media Lane Count',
    'host_lane_assignment_option': 'Host Lane Assignment Options',
    'media_lane_assignment_option': 'Media Lane Assignment Options',
    'active_apsel_hostlane1': 'Active App Selection Host Lane 1',
    'active_apsel_hostlane2': 'Active App Selection Host Lane 2',
    'active_apsel_hostlane3': 'Active App Selection Host Lane 3',
    'active_apsel_hostlane4': 'Active App Selection Host Lane 4',
    'active_apsel_hostlane5': 'Active App Selection Host Lane 5',
    'active_apsel_hostlane6': 'Active App Selection Host Lane 6',
    'active_apsel_hostlane7': 'Active App Selection Host Lane 7',
    'active_apsel_hostlane8': 'Active App Selection Host Lane 8',
    'media_interface_technology': 'Media Interface Technology',
    'cmis_rev': 'CMIS Revision',
    'supported_max_tx_power': 'Supported Max TX Power',
    'supported_min_tx_power': 'Supported Min TX Power',
    'supported_max_laser_freq': 'Supported Max Laser Frequency',
    'supported_min_laser_freq': 'Supported Min Laser Frequency'
}

SFP_DOM_CHANNEL_MONITOR_MAP = {
    'rx1power': 'RXPower',
    'tx1bias': 'TXBias',
    'tx1power': 'TXPower'
}

SFP_DOM_CHANNEL_THRESHOLD_MAP = {
    'txpowerhighalarm':   'TxPowerHighAlarm',
    'txpowerlowalarm':    'TxPowerLowAlarm',
    'txpowerhighwarning': 'TxPowerHighWarning',
    'txpowerlowwarning':  'TxPowerLowWarning',
    'rxpowerhighalarm':   'RxPowerHighAlarm',
    'rxpowerlowalarm':    'RxPowerLowAlarm',
    'rxpowerhighwarning': 'RxPowerHighWarning',
    'rxpowerlowwarning':  'RxPowerLowWarning',
    'txbiashighalarm':    'TxBiasHighAlarm',
    'txbiaslowalarm':     'TxBiasLowAlarm',
    'txbiashighwarning':  'TxBiasHighWarning',
    'txbiaslowwarning':   'TxBiasLowWarning'
}

QSFP_DOM_CHANNEL_THRESHOLD_MAP = {
    'rxpowerhighalarm':   'RxPowerHighAlarm',
    'rxpowerlowalarm':    'RxPowerLowAlarm',
    'rxpowerhighwarning': 'RxPowerHighWarning',
    'rxpowerlowwarning':  'RxPowerLowWarning',
    'txbiashighalarm':    'TxBiasHighAlarm',
    'txbiaslowalarm':     'TxBiasLowAlarm',
    'txbiashighwarning':  'TxBiasHighWarning',
    'txbiaslowwarning':   'TxBiasLowWarning'
}

DOM_MODULE_THRESHOLD_MAP = {
    'temphighalarm':  'TempHighAlarm',
    'templowalarm':   'TempLowAlarm',
    'temphighwarning': 'TempHighWarning',
    'templowwarning': 'TempLowWarning',
    'vcchighalarm':   'VccHighAlarm',
    'vcclowalarm':    'VccLowAlarm',
    'vcchighwarning': 'VccHighWarning',
    'vcclowwarning':  'VccLowWarning'
}

QSFP_DOM_CHANNEL_MONITOR_MAP = {
    'rx1power': 'RX1Power',
    'rx2power': 'RX2Power',
    'rx3power': 'RX3Power',
    'rx4power': 'RX4Power',
    'tx1bias':  'TX1Bias',
    'tx2bias':  'TX2Bias',
    'tx3bias':  'TX3Bias',
    'tx4bias':  'TX4Bias',
    'tx1power': 'TX1Power',
    'tx2power': 'TX2Power',
    'tx3power': 'TX3Power',
    'tx4power': 'TX4Power'
}

QSFP_DD_DOM_CHANNEL_MONITOR_MAP = {
    'rx1power': 'RX1Power',
    'rx2power': 'RX2Power',
    'rx3power': 'RX3Power',
    'rx4power': 'RX4Power',
    'rx5power': 'RX5Power',
    'rx6power': 'RX6Power',
    'rx7power': 'RX7Power',
    'rx8power': 'RX8Power',
    'tx1bias':  'TX1Bias',
    'tx2bias':  'TX2Bias',
    'tx3bias':  'TX3Bias',
    'tx4bias':  'TX4Bias',
    'tx5bias':  'TX5Bias',
    'tx6bias':  'TX6Bias',
    'tx7bias':  'TX7Bias',
    'tx8bias':  'TX8Bias',
    'tx1power': 'TX1Power',
    'tx2power': 'TX2Power',
    'tx3power': 'TX3Power',
    'tx4power': 'TX4Power',
    'tx5power': 'TX5Power',
    'tx6power': 'TX6Power',
    'tx7power': 'TX7Power',
    'tx8power': 'TX8Power'
}

DOM_MODULE_MONITOR_MAP = {
    'temperature': 'Temperature',
    'voltage': 'Vcc'
}

DOM_CHANNEL_THRESHOLD_UNIT_MAP = {
    'txpowerhighalarm':   'dBm',
    'txpowerlowalarm':    'dBm',
    'txpowerhighwarning': 'dBm',
    'txpowerlowwarning':  'dBm',
    'rxpowerhighalarm':   'dBm',
    'rxpowerlowalarm':    'dBm',
    'rxpowerhighwarning': 'dBm',
    'rxpowerlowwarning':  'dBm',
    'txbiashighalarm':    'mA',
    'txbiaslowalarm':     'mA',
    'txbiashighwarning':  'mA',
    'txbiaslowwarning':   'mA'
}

DOM_MODULE_THRESHOLD_UNIT_MAP = {
    'temphighalarm':   'C',
    'templowalarm':    'C',
    'temphighwarning': 'C',
    'templowwarning':  'C',
    'vcchighalarm':    'Volts',
    'vcclowalarm':     'Volts',
    'vcchighwarning':  'Volts',
    'vcclowwarning':   'Volts'
}

DOM_VALUE_UNIT_MAP = {
    'rx1power': 'dBm',
    'rx2power': 'dBm',
    'rx3power': 'dBm',
    'rx4power': 'dBm',
    'tx1bias': 'mA',
    'tx2bias': 'mA',
    'tx3bias': 'mA',
    'tx4bias': 'mA',
    'tx1power': 'dBm',
    'tx2power': 'dBm',
    'tx3power': 'dBm',
    'tx4power': 'dBm',
    'temperature': 'C',
    'voltage': 'Volts'
}

QSFP_DD_DOM_VALUE_UNIT_MAP = {
    'rx1power': 'dBm',
    'rx2power': 'dBm',
    'rx3power': 'dBm',
    'rx4power': 'dBm',
    'rx5power': 'dBm',
    'rx6power': 'dBm',
    'rx7power': 'dBm',
    'rx8power': 'dBm',
    'tx1bias': 'mA',
    'tx2bias': 'mA',
    'tx3bias': 'mA',
    'tx4bias': 'mA',
    'tx5bias': 'mA',
    'tx6bias': 'mA',
    'tx7bias': 'mA',
    'tx8bias': 'mA',
    'tx1power': 'dBm',
    'tx2power': 'dBm',
    'tx3power': 'dBm',
    'tx4power': 'dBm',
    'tx5power': 'dBm',
    'tx6power': 'dBm',
    'tx7power': 'dBm',
    'tx8power': 'dBm',
    'temperature': 'C',
    'voltage': 'Volts'
}

# Global logger instance
log = logger.Logger(SYSLOG_IDENTIFIER)

platform_chassis = None

if os.geteuid() != 0:
    click.echo("Root privileges are required for this operation")
    sys.exit(ERROR_PERMISSIONS)

try:
    platform_chassis = sonic_platform.platform.Platform().get_chassis()
except Exception as e:
    log.log_error("Failed to instantiate Chassis due to {}".format(repr(e)))

if not platform_chassis:
    sys.exit(ERROR_CHASSIS_LOAD)

def print_all_valid_port_values():
    port_list = []
    port_list.extend(range(1, platform_chassis.get_num_sfps() + 1))
    click.echo("Valid values for port: {}\n".format(str(port_list)))


