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
#include "leveling.h"
#include "rough.h"
#include "../panels.h"
#include "../main.h"
#include "../../core/ids.h"

namespace {
Panel<7> leveling_panel;

void on_back(lv_event_t *e);
void on_rough_leveling(lv_event_t *e);
void on_automatic_leveling(lv_event_t *e);
void on_reset_leveling(lv_event_t *e);
void on_z_height(lv_event_t *e);
void on_x_twist(lv_event_t *e);
void on_leveling_grid(lv_event_t *e);
}

bool LevelingPanel::can_switch() {
  return true;
}

void LevelingPanel::create() {
  if(!leveling_panel.create()) return;
  leveling_panel.layout_menu("Leveling");

  auto back = leveling_panel.control(IMG_BACK, 7, 2);
  auto rough = leveling_panel.medium_button(IMG_ROUGH, "Rough", 19, 76);
  auto automatic = leveling_panel.medium_button(IMG_AUTOMATIC, "Automatic", 101, 76);
  auto reset = leveling_panel.medium_button(IMG_RESET, " Reset", 183, 76);
  auto z_height = leveling_panel.medium_button(IMG_Z_HEIGHT, "Z-Height", 265, 76);
  auto x_twist = leveling_panel.medium_button(IMG_X_TWIST, "X Twist", 19, 188);
  auto grid = leveling_panel.medium_button(IMG_GRID, "Grid", 101, 188);

  leveling_panel.reset_bindings();
  leveling_panel.bind(back, &on_back);
  leveling_panel.bind(rough, &on_rough_leveling);
  leveling_panel.bind(automatic, &on_automatic_leveling);
  leveling_panel.bind(reset, &on_reset_leveling);
  leveling_panel.bind(z_height, &on_z_height);
  leveling_panel.bind(x_twist, &on_x_twist);
  leveling_panel.bind(grid, &on_leveling_grid);
}

namespace {

void on_back(lv_event_t *e) {
  PanelBase::switch_panel<MainPanel>();
}

void on_rough_leveling(lv_event_t *e) {
  PanelBase::switch_panel<RoughPanel>();
}

void on_automatic_leveling(lv_event_t *e) {

}

void on_reset_leveling(lv_event_t *e) {

}

void on_z_height(lv_event_t *e) {

}

void on_x_twist(lv_event_t *e) {

}

void on_leveling_grid(lv_event_t *e) {

}
}
