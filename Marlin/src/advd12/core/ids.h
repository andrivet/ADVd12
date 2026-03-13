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

static constexpr char IMG_CONTROLS[7]  = "I:0000";
static constexpr char IMG_LEVELING[7]  = "I:0001";
static constexpr char IMG_PRINT[7]     = "I:0002";
static constexpr char IMG_TUNING[7]    = "I:0003";
static constexpr char IMG_SETTINGS[7]  = "I:0004";
static constexpr char IMG_INFO[7]      = "I:0005";

static constexpr char IMG_EXTRUDER[7]  = "I:0006";
static constexpr char IMG_BED[7]       = "I:0007";
static constexpr char IMG_FAN[7]       = "I:0008";
static constexpr char IMG_TC[7]        = "I:0009";
static constexpr char IMG_ET[7]        = "I:0010";
