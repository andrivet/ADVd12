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

#include <lvgl.h>

class __FlashStringHelper;
typedef const __FlashStringHelper* FSTR_P;

namespace advd12 {

struct Core {
  static void init();
  static void task_handler();
  static void draw_error_message(FSTR_P msg);
  static void calibrate();

private:
  static void init_lvgl();
  static void init_lvgl_drivers();
  static void flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p);
  static void input_read(lv_indev_drv_t * indev, lv_indev_data_t * data);
  static void init_lvgl_styles();
  static void systick();
};

}
