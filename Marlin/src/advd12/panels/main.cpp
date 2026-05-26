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

#include "main.h"
#include "controls.h"
#include "leveling.h"
#include "../core/ids.h"

void MainPanel::show() {
  create();
  prepare();
}

void MainPanel::prepare() {
  layout_main();
  variables_time();

  auto controls = big_button(IMG_CONTROLS, "Controls", 36, 34);
  auto leveling = big_button(IMG_LEVELING, "Leveling", 140, 34);
  auto print    = big_button(IMG_PRINT, "Printing", 244, 34);
  auto tuning   = big_button(IMG_TUNING, "Tuning", 36, 154);
  auto settings = big_button(IMG_SETTINGS, "Settings", 140, 154);
  auto info     = big_button(IMG_INFO, "Info", 244, 154);

  reset_bindings();
  bind(controls, &MainPanel::on_controls);
  bind(leveling, &MainPanel::on_leveling);
  bind(print, &MainPanel::on_print);
  bind(tuning, &MainPanel::on_tuning);
  bind(settings, &MainPanel::on_settings);
  bind(info, &MainPanel::on_info);
}

void MainPanel::on_controls(lv_event_t *e) {
  switch_panel(controls_panel);
}

void MainPanel::on_leveling(lv_event_t *e) {
  switch_panel(leveling_panel);
}

void MainPanel::on_print(lv_event_t *e) {

}

void MainPanel::on_tuning(lv_event_t *e) {

}

void MainPanel::on_settings(lv_event_t *e) {

}

void MainPanel::on_info(lv_event_t *e) {

}
