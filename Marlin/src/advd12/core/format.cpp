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

#include "format.h"
#include <cmath>
#include <cstdio>
#include <cstring>

namespace Format {

void format0(char *buffer, std::size_t buffer_size, double value) {
  if(std::isnan(value)) {
    strncpy(buffer, "-", buffer_size);
    return;
  }
  auto rounded = ::lround(value * 10);
  auto negative = rounded < 0;
  if(negative) rounded = -rounded;
  auto whole = rounded / 10;
  auto fract = rounded % 10;
  snprintf(buffer, buffer_size, negative ? "-%ld.%ld" : "%ld.%ld", whole, fract);
}

void format00(char *buffer, std::size_t buffer_size, double value) {
  if(std::isnan(value)) {
    strncpy(buffer, "-", buffer_size);
    return;
  }
  auto rounded = ::lround(value * 100);
  auto negative = rounded < 0;
  if(negative) rounded = -rounded;
  auto whole = rounded / 100;
  auto fract = rounded % 100;
  snprintf(buffer, buffer_size, negative ? "-%ld.%ld" : "%ld.%ld", whole, fract);
}

}
