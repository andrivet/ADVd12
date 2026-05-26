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


#include "leveling.h"
#include "main.h"
#include "../core/ids.h"

void LevelingPanel::prepare() {
  layout_menu("Leveling");
  variables_time();

  auto back = control(IMG_BACK, 7, 2);

  auto rough   = medium_button(IMG_ROUGH, "Rough", 19, 76);
  auto automatic = medium_button(IMG_AUTOMATIC, "Automatic", 101, 76);
  auto reset   = medium_button(IMG_RESET, " Reset", 183, 76);
  auto z_height   = medium_button(IMG_Z_HEIGHT, "Z-Height", 265, 76);
  auto x_twist= medium_button(IMG_X_TWIST, "X Twist", 19, 188);
  auto grid= medium_button(IMG_GRID, "Grid", 19, 188);

  reset_bindings();
  bind(back, &LevelingPanel::on_back);
  bind(rough, &LevelingPanel::on_rough_leveling);
  bind(automatic, &LevelingPanel::on_automatic_leveling);
  bind(reset, &LevelingPanel::on_reset_leveling);
  bind(z_height, &LevelingPanel::on_z_height);
  bind(x_twist, &LevelingPanel::on_x_twist);
  bind(grid, &LevelingPanel::on_leveling_grid);
}

void LevelingPanel::on_back(lv_event_t *e) {
  switch_panel(main_panel);
}

void LevelingPanel::on_rough_leveling(lv_event_t *e) {

}

void LevelingPanel::on_automatic_leveling(lv_event_t *e) {

}

void LevelingPanel::on_reset_leveling(lv_event_t *e) {

}

void LevelingPanel::on_z_height(lv_event_t *e) {

}

void LevelingPanel::on_x_twist(lv_event_t *e) {

}

void LevelingPanel::on_leveling_grid(lv_event_t *e) {

}
