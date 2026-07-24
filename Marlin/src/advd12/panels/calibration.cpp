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

#include "calibration.h"
#include "main.h"
#include "panels.h"
#include "../core/styles.h"
#include "../../lcd/tft_io/touch_calibration.h"

namespace {
Panel<0> panel;

lv_obj_t *line(lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height);

lv_obj_t *cross_line1_ = nullptr;
lv_obj_t *cross_line2_ = nullptr;
}

bool CalibrationPanel::can_switch() {
  return true;
}

void CalibrationPanel::create() {
  if(!panel.create()) return;

  TouchCalibration::calibration_start();
  auto pt = TouchCalibration::calibration_points[0];
  cross_line1_ = line(pt.x - 15, pt.y, 31, 1);
  cross_line2_ = line(pt.x, pt.y - 15, 1, 31);

  auto label = lv_label_create(panel.scr());
  lv_label_set_text(label, "LCD Calibration");
  lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
  lv_obj_align_to(label, nullptr, LV_ALIGN_CENTER, 0, 0);
}

void CalibrationPanel::update() {
  auto stage = TouchCalibration::get_calibration_state();

  if(stage == CALIBRATION_FAIL) {
    auto label = lv_label_create(panel.scr());
    lv_label_set_text(label, "Calibration Failed");
    lv_obj_set_style_text_color(label, red, LV_PART_MAIN);
    lv_obj_align_to(label, nullptr, LV_ALIGN_CENTER, 0, 24);
    return;
  }

  if(stage == CALIBRATION_SUCCESS) {
    TouchCalibration::calibration_end();
    switch_panel<MainPanel>();
    return;
  }

  auto pt = TouchCalibration::calibration_points[stage];
  lv_obj_set_pos(cross_line1_, pt.x - 15, pt.y);
  lv_obj_set_pos(cross_line2_, pt.x, pt.y - 15);
}

namespace {
lv_obj_t *line(lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height) {
  auto rect = lv_obj_create(panel.scr());
  lv_obj_set_pos(rect, x, y);
  lv_obj_set_size(rect, width, height);
  lv_obj_set_style_bg_color(rect, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(rect, LV_OPA_COVER, LV_PART_MAIN);
  return rect;
}
}
