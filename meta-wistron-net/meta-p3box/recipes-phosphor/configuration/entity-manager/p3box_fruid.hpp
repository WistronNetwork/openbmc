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

struct frupos {
    uint32_t bus;
    uint32_t addr;
};

bool operator< (frupos a, frupos b) \
    {return std::make_pair(a.bus,a.addr) < std::make_pair(b.bus,b.addr) ;}

std::map<frupos, int> fruid_map = {
    {{8, 0x51}, 0},
    {{18, 0x50}, 1},
    {{24, 0x51}, 2},
    {{24, 0x53}, 3},
    {{24, 0x55}, 4},
    {{25, 0x51}, 5},
    {{25, 0x53}, 6},
    {{25, 0x55}, 7},
    {{17, 0x50}, 8},
    {{17, 0x51}, 9}
};
