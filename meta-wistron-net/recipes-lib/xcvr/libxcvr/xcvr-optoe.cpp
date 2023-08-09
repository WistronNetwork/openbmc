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

#define LOWER_MEMORY_SIZE 128
#define UPPER_MEMORY_SIZE 128

XcvrOptoe::XcvrOptoe(const char* filename) : filename_(filename) {}

int XcvrOptoe::read_raw(uint8_t page, uint8_t offset,
                         int length, uint8_t* data) {
    int fd;
    int ret = 0;
    int eeprom_offset;

    fd = open(filename_, O_RDONLY);
    if (fd < 0) {
        std::cerr << "Failed to open XCVR EEPROM file: "
                  << filename_ << std::endl;
        goto error;
    }

    if (offset < 128) {
        eeprom_offset = offset;

        if (lseek(fd, eeprom_offset, SEEK_SET) != eeprom_offset) {
            std::cerr << "Failed to seek XCVR EEPROM file: "
                        << filename_ << std::endl;
            ret = -1;
            goto error;
        }

        if (length <= LOWER_MEMORY_SIZE - offset) {
            ret = file_read_bytes(fd, data, length);
            if (ret != length) {
                std::cerr << "Failed to read XCVR EEPROM file: "
                            << filename_ << std::endl;
                goto error;
            }
            ret = 0;
        } else {
            ret = file_read_bytes(fd, data, LOWER_MEMORY_SIZE - offset);
            if (ret != LOWER_MEMORY_SIZE - offset) {
                std::cerr << "Failed to read XCVR EEPROM file: "
                            << filename_ << std::endl;
                goto error;
            }
            lseek(fd, 0, SEEK_SET);

            eeprom_offset = LOWER_MEMORY_SIZE + (page * UPPER_MEMORY_SIZE);
            if (lseek(fd, eeprom_offset, SEEK_SET) != eeprom_offset) {
                std::cerr << "Failed to seek XCVR EEPROM file: "
                            << filename_ << std::endl;
                ret = -1;
                goto error;
            }

            ret = file_read_bytes(fd, &data[LOWER_MEMORY_SIZE - offset],
                                  length - LOWER_MEMORY_SIZE);
            if (ret != length - LOWER_MEMORY_SIZE) {
                std::cerr << "Failed to read XCVR EEPROM file: "
                            << filename_ << std::endl;
                goto error;
            }
            ret = 0;
        }
    } else {
        eeprom_offset = LOWER_MEMORY_SIZE  + page * UPPER_MEMORY_SIZE +
                        (offset - LOWER_MEMORY_SIZE);
        if (lseek(fd, eeprom_offset, SEEK_SET) != eeprom_offset) {
            std::cerr << "Failed to seek XCVR EEPROM file: "
                    << filename_ << std::endl;
            ret = -1;
            goto error;
        }

        if (length > UPPER_MEMORY_SIZE - (offset - LOWER_MEMORY_SIZE))
            length = UPPER_MEMORY_SIZE + LOWER_MEMORY_SIZE - offset;

        if (file_read_bytes(fd, data, length) != length) {
            std::cerr << "Failed to read XCVR EEPROM file: "
                    << filename_ << std::endl;
            ret = -1;
            goto error;
        }
    }
error:
    close(fd);

    return ret;
}

int XcvrOptoe::write_raw(uint8_t page, uint8_t offset,
                          int length, const uint8_t* data) {
    int fd;
    int ret = 0;
    int eeprom_offset;

    fd = open(filename_, O_RDWR);
    if (fd < 0) {
        std::cerr << "Failed to open XCVR EEPROM file: "
                  << filename_ << std::endl;
        return -1;
    }

    if (offset < 128) {
        eeprom_offset = offset;

        if (lseek(fd, eeprom_offset, SEEK_SET) != eeprom_offset) {
            std::cerr << "Failed to seek XCVR EEPROM file: "
                        << filename_ << std::endl;
            ret = -1;
            goto error;
        }

        if (length <= LOWER_MEMORY_SIZE - offset) {
            ret = file_write_bytes(fd, data, length);
            if (ret != length) {
                std::cerr << "Failed to write XCVR EEPROM file: "
                            << filename_ << std::endl;
                goto error;
            }
            ret = 0;
        } else {
            ret = file_write_bytes(fd, data, LOWER_MEMORY_SIZE - offset);
            if (ret != LOWER_MEMORY_SIZE - offset) {
                std::cerr << "Failed to write XCVR EEPROM file: "
                            << filename_ << std::endl;
                goto error;
            }
            lseek(fd, 0, SEEK_SET);

            eeprom_offset = LOWER_MEMORY_SIZE + (page * UPPER_MEMORY_SIZE);
            if (lseek(fd, eeprom_offset, SEEK_SET) != eeprom_offset) {
                std::cerr << "Failed to seek XCVR EEPROM file: "
                            << filename_ << std::endl;
                ret = -1;
                goto error;
            }

            ret = file_write_bytes(fd, &data[LOWER_MEMORY_SIZE - offset],
                                  length - LOWER_MEMORY_SIZE);
            if (ret != length - LOWER_MEMORY_SIZE) {
                std::cerr << "Failed to write XCVR EEPROM file: "
                            << filename_ << std::endl;
                goto error;
            }
            ret = 0;
        }
    } else {
        eeprom_offset = LOWER_MEMORY_SIZE  + page * UPPER_MEMORY_SIZE +
                        (offset - LOWER_MEMORY_SIZE);
        if (lseek(fd, eeprom_offset, SEEK_SET) != eeprom_offset) {
            std::cerr << "Failed to seek XCVR EEPROM file: "
                    << filename_ << std::endl;
            ret = -1;
            goto error;
        }

        if (length > UPPER_MEMORY_SIZE - (offset - LOWER_MEMORY_SIZE))
            length = UPPER_MEMORY_SIZE + LOWER_MEMORY_SIZE - offset;

        if (file_write_bytes(fd, data, length) != length) {
            std::cerr << "Failed to write XCVR EEPROM file: "
                    << filename_ << std::endl;
            ret = -1;
            goto error;
        }
    }
error:
    close(fd);

    return ret;
}
