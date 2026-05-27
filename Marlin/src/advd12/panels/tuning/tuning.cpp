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


#include "tuning.h"
#include "../main.h"
#include "../../core/ids.h"

void TuningPanel::prepare() {
  layout_menu("Tuning");
  variables_time();

  auto back = control(IMG_BACK, 7, 2);

  auto sensor = medium_button(IMG_SENSOR, "BLTouch", 19, 76);
  auto extruder = medium_button(IMG_EXTRUDER2, "Extruder", 101, 76);
  auto pid = medium_button(IMG_PID, " PID", 183, 76);
  auto vibrations = medium_button(IMG_VIBRATIONS, "Vibrations", 265, 76);
  auto shaping = medium_button(IMG_SHAPING, "X Shaping", 19, 188);
  auto skew = medium_button(IMG_SKEW, "Skew", 19, 188);
  auto linear_adv = medium_button(IMG_LINEAR_ADV, "Linear Adv", 101, 188);

  reset_bindings();
  bind(back, &TuningPanel::on_back);
  bind(sensor, &TuningPanel::on_bltouch);
  bind(extruder, &TuningPanel::on_extruder);
  bind(pid, &TuningPanel::on_pid);
  bind(vibrations, &TuningPanel::on_vibrations);
  bind(shaping, &TuningPanel::on_shaping);
  bind(skew, &TuningPanel::on_skew);
  bind(linear_adv, &TuningPanel::on_linear_advance);
}

void TuningPanel::on_back(lv_event_t *e) {
  switch_panel(main_panel);
}

void TuningPanel::on_bltouch(lv_event_t *e) {

}

void TuningPanel::on_extruder(lv_event_t *e) {

}

void TuningPanel::on_pid(lv_event_t *e) {

}

void TuningPanel::on_vibrations(lv_event_t *e) {

}

void TuningPanel::on_shaping(lv_event_t *e) {

}

void TuningPanel::on_skew(lv_event_t *e) {

}

void TuningPanel::on_linear_advance(lv_event_t *e) {

}
