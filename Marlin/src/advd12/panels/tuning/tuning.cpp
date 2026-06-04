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

#include <lvgl.h>
#include "tuning.h"
#include "../main.h"
#include "../../core/ids.h"
#include "src/advd12/panels/panels.h"

namespace {
Panel<8> tuning_panel;

void on_back(lv_event_t *e);
void on_bltouch(lv_event_t *e);
void on_extruder(lv_event_t *e);
void on_pid(lv_event_t *e);
void on_vibrations(lv_event_t *e);
void on_shaping(lv_event_t *e);
void on_skew(lv_event_t *e);
void on_linear_advance(lv_event_t *e);
}

bool TuningPanel::can_switch() {
  return true;
}

void TuningPanel::create() {
  if(!tuning_panel.create()) return;
  tuning_panel.layout_menu("Tuning");
  tuning_panel.variables_time();

  auto back = tuning_panel.control(IMG_BACK, 7, 2);

  auto sensor = tuning_panel.medium_button(IMG_SENSOR, "BLTouch", 19, 76);
  auto extruder = tuning_panel.medium_button(IMG_EXTRUDER2, "Extruder", 101, 76);
  auto pid = tuning_panel.medium_button(IMG_PID, " PID", 183, 76);
  auto vibrations = tuning_panel.medium_button(IMG_VIBRATIONS, "Vibrations", 265, 76);
  auto shaping = tuning_panel.medium_button(IMG_SHAPING, "X Shaping", 19, 188);
  auto skew = tuning_panel.medium_button(IMG_SKEW, "Skew", 19, 188);
  auto linear_adv = tuning_panel.medium_button(IMG_LINEAR_ADV, "Linear Adv", 101, 188);

  tuning_panel.reset_bindings();
  tuning_panel.bind(back, &on_back);
  tuning_panel.bind(sensor, &on_bltouch);
  tuning_panel.bind(extruder, &on_extruder);
  tuning_panel.bind(pid, &on_pid);
  tuning_panel.bind(vibrations, &on_vibrations);
  tuning_panel.bind(shaping, &on_shaping);
  tuning_panel.bind(skew, &on_skew);
  tuning_panel.bind(linear_adv, &on_linear_advance);
}

namespace {

void on_back(lv_event_t *e) {
  PanelBase::switch_panel<MainPanel>();
}

void on_bltouch(lv_event_t *e) {

}

void on_extruder(lv_event_t *e) {

}

void on_pid(lv_event_t *e) {

}

void on_vibrations(lv_event_t *e) {

}

void on_shaping(lv_event_t *e) {

}

void on_skew(lv_event_t *e) {

}

void on_linear_advance(lv_event_t *e) {

}

}