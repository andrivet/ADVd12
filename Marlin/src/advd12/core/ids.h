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

// To be synchronized with Sketch file

inline constexpr char IMG_EXTRUDER[7]     = "I:0000";
inline constexpr char IMG_BED[7]          = "I:0001";
inline constexpr char IMG_FAN[7]          = "I:0002";
inline constexpr char IMG_TC[7]           = "I:0003";
inline constexpr char IMG_ET[7]           = "I:0004";
inline constexpr char IMG_PERCENT[7]      = "I:0005";
inline constexpr char IMG_LEFT[7]         = "I:0006";
inline constexpr char IMG_CONTROLS[7]     = "I:0007";
inline constexpr char IMG_LEVELING[7]     = "I:0008";
inline constexpr char IMG_PRINT[7]        = "I:0009";
inline constexpr char IMG_TUNING[7]       = "I:0010";
inline constexpr char IMG_SETTINGS[7]     = "I:0011";
inline constexpr char IMG_INFO[7]         = "I:0012";
inline constexpr char IMG_LOAD[7]         = "I:0013";
inline constexpr char IMG_HEAT[7]         = "I:0014";
inline constexpr char IMG_COOL[7]         = "I:0015";
inline constexpr char IMG_MOVE[7]         = "I:0016";
inline constexpr char IMG_MOTOR[7]        = "I:0017";
inline constexpr char IMG_ROUGH[7]        = "I:0018";
inline constexpr char IMG_AUTOMATIC[7]    = "I:0019";
inline constexpr char IMG_RESET[7]        = "I:0020";
inline constexpr char IMG_Z_HEIGHT[7]     = "I:0021";
inline constexpr char IMG_X_TWIST[7]      = "I:0022";
inline constexpr char IMG_GRID[7]         = "I:0023";
inline constexpr char IMG_MANUAL[7]       = "I:0024";
inline constexpr char IMG_SENSOR[7]       = "I:0025";
inline constexpr char IMG_EXTRUDER2[7]    = "I:0026";
inline constexpr char IMG_PID[7]          = "I:0027";
inline constexpr char IMG_VIBRATIONS[7]   = "I:0028";
inline constexpr char IMG_SHAPING[7]      = "I:0029";
inline constexpr char IMG_SKEW[7]         = "I:0030";
inline constexpr char IMG_LINEAR_ADV[7]   = "I:0031";
inline constexpr char IMG_MOTORS[7]       = "I:0032";
inline constexpr char IMG_SENSORS[7]      = "I:0033";
inline constexpr char IMG_BRIGHTNESS[7]   = "I:0034";
inline constexpr char IMG_BEEPER[7]       = "I:0035";
inline constexpr char IMG_RUNOUT[7]       = "I:0036";
inline constexpr char IMG_STATS[7]        = "I:0037";
inline constexpr char IMG_VERSIONS[7]     = "I:0038";
inline constexpr char IMG_COPYRIGHTS[7]   = "I:0039";
inline constexpr char IMG_IO[7]           = "I:0040";
inline constexpr char IMG_STEPS[7]        = "I:0041";
inline constexpr char IMG_FEEDRATE[7]     = "I:0042";
inline constexpr char IMG_ACCELERATION[7] = "I:0043";
inline constexpr char IMG_MINUS[7]        = "I:0044";
inline constexpr char IMG_PLUS[7]         = "I:0045";
inline constexpr char IMG_UNLOAD[7]       = "I:0046";
inline constexpr char IMG_LOAD2[7]        = "I:0047";
inline constexpr char IMG_SAVE[7]         = "I:0048";
inline constexpr char IMG_RESET2[7]       = "I:0049";
//inline constexpr char IMG_NEXT[7]         = "I:0050";
inline constexpr char IMG_COOLDOWN[7]     = "I:0051";
inline constexpr char IMG_DISABLE[7]      = "I:0052";
inline constexpr char IMG_HOME[7]         = "I:0053";
inline constexpr char IMG_DOWN[7]         = "I:0054";
inline constexpr char IMG_UP[7]           = "I:0055";
inline constexpr char IMG_CIRCLE[7]       = "I:0056";
inline constexpr char IMG_RIGHT[7]        = "I:0057";
inline constexpr char IMG_CONFIRM[7]      = "I:0058";
inline constexpr char IMG_FS_EMPTY[7]     = "I:0059";
inline constexpr char IMG_FS_FILE[7]      = "I:0060";
inline constexpr char IMG_FS_FOLDER[7]    = "I:0061";
inline constexpr char IMG_STOP[7]         = "I:0062";
inline constexpr char IMG_PAUSE[7]        = "I:0063";
inline constexpr char IMG_CHANGE[7]       = "I:0064";
inline constexpr char IMG_OPTIONS[7]      = "I:0065";
inline constexpr char IMG_STEPS2[7]       = "I:0066";
inline constexpr char IMG_PROBE[7]        = "I:0067";
inline constexpr char IMG_START[7]        = "I:0068";
inline constexpr char IMG_FEEDRATE2[7]    = "I:0069";
inline constexpr char IMG_FLOWRATE[7]     = "I:0070";
inline constexpr char IMG_CANCEL[7]       = "I:0071";
