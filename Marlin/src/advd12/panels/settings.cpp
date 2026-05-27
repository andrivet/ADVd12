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


#include "settings.h"
#include "main.h"
#include "../core/ids.h"

void SettingsPanel::prepare() {
  layout_menu("Settings");
  variables_time();

  auto back = control(IMG_BACK, 7, 2);

  auto motors = medium_button(IMG_MOTOR, "Motors", 19, 76);
  auto pid = medium_button(IMG_PID, "PID", 101, 76);
  auto sensor = medium_button(IMG_SENSOR, " Sensor", 183, 76);
  auto brightness = medium_button(IMG_BRIGHTNESS, "LCD", 265, 76);
  auto beeper = medium_button(IMG_BEEPER, "X Beeper", 19, 188);
  auto runout = medium_button(IMG_RUNOUT, "Runout", 19, 188);

  reset_bindings();
  bind(back, &SettingsPanel::on_back);
  bind(motors, &SettingsPanel::on_motors);
  bind(pid, &SettingsPanel::on_pid);
  bind(sensor, &SettingsPanel::on_sensor);
  bind(brightness, &SettingsPanel::on_brightness);
  bind(beeper, &SettingsPanel::on_beeper);
  bind(runout, &SettingsPanel::on_runout);
}

void SettingsPanel::on_back(lv_event_t *e) {
  switch_panel(main_panel);
}

void SettingsPanel::on_motors(lv_event_t *e) {

}

void SettingsPanel::on_pid(lv_event_t *e) {

}

void SettingsPanel::on_sensor(lv_event_t *e) {

}

void SettingsPanel::on_brightness(lv_event_t *e) {

}

void SettingsPanel::on_beeper(lv_event_t *e) {

}

void SettingsPanel::on_runout(lv_event_t *e) {

}
