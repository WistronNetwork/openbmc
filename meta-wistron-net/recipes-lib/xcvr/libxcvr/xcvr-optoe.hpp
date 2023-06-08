/*
 * Copyright 2022-present Wistron. All Rights Reserved.
 *
 */

#pragma once

#include <stdint.h>

class XcvrOptoe {
public:
    XcvrOptoe(const char* filename);

    int read_raw(uint8_t page, uint8_t offset, int length, uint8_t* data);
    int write_raw(uint8_t page, uint8_t offset, int length, const uint8_t* data);

private:
    const char* filename_;
};
