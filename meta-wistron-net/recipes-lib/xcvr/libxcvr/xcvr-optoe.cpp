/*
 * Copyright 2022-present Wistron. All Rights Reserved.
 *
 */

#include <iostream>
#include <fstream>
#include <openbmc/misc-utils.h>
#include <fcntl.h>
#include <unistd.h>
#include "xcvr-optoe.hpp"

XcvrOptoe::XcvrOptoe(const char* filename) : filename_(filename) {}

int XcvrOptoe::read_raw(uint8_t page, uint8_t offset,
                         int length, uint8_t* data) {
    int fd;
    int eeprom_offset;

    fd = open(filename_, O_RDONLY);
    if (fd < 0) {
        std::cerr << "Failed to open XCVR EEPROM file: "
                  << filename_ << std::endl;
        close(fd);
        return -1;
    }

    eeprom_offset = (page * 128) + offset;
    if (lseek(fd, eeprom_offset, SEEK_SET) != eeprom_offset) {
        std::cerr << "Failed to seek XCVR EEPROM file: "
                  << filename_ << std::endl;
        close(fd);
        return -1;
    }

    if (file_read_bytes(fd, data, length) != length) {
        std::cerr << "Failed to read XCVR EEPROM file: "
                  << filename_ << std::endl;
        close(fd);
        return -1;
    }

    close(fd);

    return 0;
}

int XcvrOptoe::write_raw(uint8_t page, uint8_t offset,
                          int length, const uint8_t* data) {
    int fd;
    int eeprom_offset;

    fd = open(filename_, O_RDWR);
    if (fd < 0) {
        std::cerr << "Failed to open XCVR EEPROM file: "
                  << filename_ << std::endl;
        return -1;
    }

    eeprom_offset = (page * 128) + offset;
    if (lseek(fd, eeprom_offset, SEEK_SET) != eeprom_offset) {
        std::cerr << "Failed to seek XCVR EEPROM file: "
                  << filename_ << std::endl;
        close(fd);
        return -1;
    }

    if (file_write_bytes(fd, data, length) != length) {
        std::cerr << "Failed to write XCVR EEPROM file: "
                  << filename_ << std::endl;
        close(fd);
        return -1;
    }

    return 0;
}
