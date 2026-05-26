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

// To be synchronized with adv_encode_assets.py

inline constexpr char IMG_CONTROLS[7]  = "I:0000";
inline constexpr char IMG_LEVELING[7]  = "I:0001";
inline constexpr char IMG_PRINT[7]     = "I:0002";
inline constexpr char IMG_TUNING[7]    = "I:0003";
inline constexpr char IMG_SETTINGS[7]  = "I:0004";
inline constexpr char IMG_INFO[7]      = "I:0005";

inline constexpr char IMG_EXTRUDER[7]  = "I:0006";
inline constexpr char IMG_BED[7]       = "I:0007";
inline constexpr char IMG_FAN[7]       = "I:0008";
inline constexpr char IMG_TC[7]        = "I:0009";
inline constexpr char IMG_ET[7]        = "I:0010";
inline constexpr char IMG_PERCENT[7]   = "I:0011";
inline constexpr char IMG_BACK[7]      = "I:0012";

inline constexpr char IMG_LOAD[7]      = "I:0013";
inline constexpr char IMG_HEAT[7]      = "I:0014";
inline constexpr char IMG_COOL[7]      = "I:0015";
inline constexpr char IMG_MOVE[7]      = "I:0016";
inline constexpr char IMG_MOTOR[7]     = "I:0017";
