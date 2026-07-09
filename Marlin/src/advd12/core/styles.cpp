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

#include "styles.h"
#include "display.h"

void Display::init_lvgl_styles() {
  lv_style_init(&style_dropdown);
  lv_style_set_bg_color(&style_dropdown, lv_color_black());
  lv_style_set_bg_opa(&style_dropdown, LV_OPA_COVER);
  lv_style_set_border_color(&style_dropdown, orange);
  lv_style_set_border_width(&style_dropdown, 2);
  lv_style_set_text_color(&style_dropdown, orange);
  lv_style_set_pad_all(&style_dropdown, 8);
  lv_style_set_text_line_space(&style_dropdown, 16);
}
