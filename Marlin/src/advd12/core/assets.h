/**
 * ADVd12 firmware
 * Copyright (c) 2026 Sebastien Andriver [https://github.com/andrivet/advd12]
 *
 * Base on Marlin firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#pragma once
#include <cstdint>

// Size of the W25Q64FV flash: 8 Mbytes (64 Mbits), i.e. 0x800000

static constexpr uint32_t ADDR_SIGNATURE  = 0x000000; // 4 bytes
static constexpr uint32_t ADDR_LOGO       = 0x000004; // 64 KBytes
static constexpr uint32_t ADDR_IMAGES     = 0x050000;
static constexpr uint32_t ADDR_ADDRS      = 0x7E0000; // 1024 bytes (256 images)

static constexpr uint32_t SIGNATURE       = 0x30564441; // ADV0 (little-endian)
static constexpr uint32_t SIZE_ASSETS     = 0x800000; // 8 MBytes
static constexpr uint32_t SIZE_BLOCK      = 64 * 1024; // 64 KBytes
