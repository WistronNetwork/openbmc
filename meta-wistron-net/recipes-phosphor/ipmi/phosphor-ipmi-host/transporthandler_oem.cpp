/* Copyright 2022-present Wistron. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "transporthandler.hpp"

namespace ipmi::transport
{

/** @brief Sets the system value for MAC volatile attribute.
 *
 *  @param[in] bus        - The bus object used for lookups
 *  @param[in] params     - The parameters for the channel
 *  @param[in] isnonvolatile - MAC volatile attribute.
 */
void setMACVolatileProperty(sdbusplus::bus::bus& bus,
                            const ChannelParams& params,
                            const bool isnonvolatile)
{
    setDbusProperty(bus, params.service, params.ifPath, INTF_MAC,
                    "MacNonVolatile", isnonvolatile);
}

RspType<> setLanOem(uint8_t channel, uint8_t parameter, message::Payload& req)
{
    req.trailingOk = true;

    if (parameter == 0xc1) {
        channelCall<setMACVolatileProperty>(channel, 0);
    }

    if (parameter == 0xc2) {
        channelCall<setMACVolatileProperty>(channel, 1);
    }

    return responseSuccess();
}
} // namespace ipmi::transport
