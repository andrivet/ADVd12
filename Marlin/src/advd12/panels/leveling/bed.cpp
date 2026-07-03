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

#include "bed.h"

void Bed::create(lv_obj_t *parent) {
  auto gray = lv_color_make(0x8C, 0x8C, 0x8C);

  auto rect = lv_obj_create(parent);
  lv_obj_set_pos(rect, 26, 227);
  lv_obj_set_size(rect, 293, 28);
  lv_obj_set_style_bg_color(rect, gray, LV_PART_MAIN);
  lv_obj_set_style_bg_opa(rect, LV_OPA_COVER, LV_PART_MAIN);

  static const lv_point_t points[] = {
  {27 +   0, 74 + 151},
  {27 + 290, 74 + 151},
  {27 + 246, 74 + 0},
  {27 +  44, 74 + 0},
  {27 +   0, 74 + 151}
  };

  auto line = lv_line_create(parent);
  lv_line_set_points(line, points, 5);
  //lv_obj_set_size(line, 290, 151);

  lv_obj_set_style_line_width(line, 3, LV_PART_MAIN);
  lv_obj_set_style_line_color(line, gray, LV_PART_MAIN);
  lv_obj_set_style_line_rounded(line, true, LV_PART_MAIN);
}