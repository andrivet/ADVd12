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
#include "motors.h"
#include "../main.h"
#include "../../core/ids.h"
#include "src/advd12/panels/panels.h"

namespace {
Panel<5> panel;

void on_back(lv_event_t *e);
void on_steps(lv_event_t *e);
void on_feedrate(lv_event_t *e);
void on_max_acceleration(lv_event_t *e);
void on_default_acceleration(lv_event_t *e);
}

bool MotorsPanel::can_switch() {
  return true;
}

void MotorsPanel::create() {
  if(!panel.create()) return;
  panel.layout_menu("Motors");

  auto back = panel.control(IMG_LEFT, 7, 2);

  auto steps = panel.medium_button(IMG_STEPS, "Steps", 19, 76);
  auto feedrate = panel.medium_button(IMG_FEEDRATE, "Feedrate", 101, 76);
  auto max_accel = panel.medium_button(IMG_ACCELERATION, "Max Accel", 183, 76);
  auto def_accel = panel.medium_button(IMG_ACCELERATION, "Def Accel", 265, 76);

  panel.reset_bindings();
  panel.bind(back, &on_back);
  panel.bind(steps, &on_steps);
  panel.bind(feedrate, &on_feedrate);
  panel.bind(max_accel, &on_max_acceleration);
  panel.bind(def_accel, &on_default_acceleration);
}

namespace {

void on_back(lv_event_t *e) {
  switch_panel<MainPanel>();
}

void on_steps(lv_event_t *e) {

}

void on_feedrate(lv_event_t *e) {

}

void on_max_acceleration(lv_event_t *e) {

}

void on_default_acceleration(lv_event_t *e) {

}

}
