/*
// Copyright (c) 2022 Wistron Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/

#pragma once

#include <map>

#define FRU_BMC 0xff

struct frupos {
    uint32_t bus;
    uint32_t addr;
};

bool operator< (frupos a, frupos b) \
    {return std::make_pair(a.bus,a.addr) < std::make_pair(b.bus,b.addr) ;}

std::map<frupos, int> fruid_map = {
    {{0xff, 0xff}, 0xff},
};
