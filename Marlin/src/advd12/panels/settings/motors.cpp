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
Panel<5> motors_panel;

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
  if(!motors_panel.create()) return;
  motors_panel.layout_menu("Motors");

  auto back = motors_panel.control(IMG_LEFT, 7, 2);

  auto steps = motors_panel.medium_button(IMG_STEPS, "Steps", 19, 76);
  auto feedrate = motors_panel.medium_button(IMG_FEEDRATE, "Feedrate", 101, 76);
  auto max_accel = motors_panel.medium_button(IMG_ACCELERATION, "Max Accel", 183, 76);
  auto def_accel = motors_panel.medium_button(IMG_ACCELERATION, "Def Accel", 265, 76);

  motors_panel.reset_bindings();
  motors_panel.bind(back, &on_back);
  motors_panel.bind(steps, &on_steps);
  motors_panel.bind(feedrate, &on_feedrate);
  motors_panel.bind(max_accel, &on_max_acceleration);
  motors_panel.bind(def_accel, &on_default_acceleration);
}

namespace {

void on_back(lv_event_t *e) {
  PanelBase::switch_panel<MainPanel>();
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
