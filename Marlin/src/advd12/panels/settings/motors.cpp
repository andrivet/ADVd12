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


#include "motors.h"
#include "../main.h"
#include "../../core/ids.h"

void MotorsPanel::prepare() {
  layout_menu("Motors");
  variables_time();

  auto back = control(IMG_BACK, 7, 2);

  auto steps = medium_button(IMG_STEPS, "Steps", 19, 76);
  auto feedrate = medium_button(IMG_FEEDRATE, "Feedrate", 101, 76);
  auto max_accel = medium_button(IMG_ACCELERATION, "Max Accel", 183, 76);
  auto def_accel = medium_button(IMG_ACCELERATION, "Def Accel", 265, 76);

  reset_bindings();
  bind(back, &MotorsPanel::on_back);
  bind(steps, &MotorsPanel::on_steps);
  bind(feedrate, &MotorsPanel::on_feedrate);
  bind(max_accel, &MotorsPanel::on_max_acceleration);
  bind(def_accel, &MotorsPanel::on_default_acceleration);
}

void MotorsPanel::on_back(lv_event_t *e) {
  switch_panel(main_panel);
}

void MotorsPanel::on_steps(lv_event_t *e) {

}

void MotorsPanel::on_feedrate(lv_event_t *e) {

}

void MotorsPanel::on_max_acceleration(lv_event_t *e) {

}

void MotorsPanel::on_default_acceleration(lv_event_t *e) {

}
