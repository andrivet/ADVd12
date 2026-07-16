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

#include "../../../inc/MarlinConfig.h"
#if HAS_LEVELING
#include "../../../feature/bltouch.h"
#endif
#include <lvgl.h>
#include "leveling.h"
#include "rough.h"
#include "automatic.h"
#include "grid.h"
#include "zheight.h"
#include "../panels.h"
#include "../main.h"
#include "../../core/ids.h"

namespace {
Panel<7> panel;

void on_back(lv_event_t *e);
void on_rough_leveling(lv_event_t *e);
#if HAS_LEVELING
void on_automatic_leveling(lv_event_t *e);
void on_reset_leveling(lv_event_t *e);
void on_z_height(lv_event_t *e);
void on_leveling_grid(lv_event_t *e);
#endif
}

bool LevelingPanel::can_switch() {
  return true;
}

void LevelingPanel::create() {
  panel.create();
  panel.layout_menu("Leveling");

  auto back = panel.control(IMG_LEFT, 7, 2);
  auto rough = panel.medium_button(IMG_ROUGH, "Rough", 19, 76);

#if HAS_LEVELING
  auto automatic = panel.medium_button(IMG_AUTOMATIC, "Automatic", 101, 76);
  auto reset = panel.medium_button(IMG_RESET, " Reset", 183, 76);
  auto z_height = panel.medium_button(IMG_Z_HEIGHT, "Z-Height", 265, 76);
  auto grid = panel.medium_button(IMG_GRID, "Grid", 19, 188);
#endif

  panel.reset_bindings();
  panel.bind(back, &on_back);
  panel.bind(rough, &on_rough_leveling);
#if HAS_LEVELING
  panel.bind(automatic, &on_automatic_leveling);
  panel.bind(reset, &on_reset_leveling);
  panel.bind(z_height, &on_z_height);
  panel.bind(grid, &on_leveling_grid);
#endif
}

namespace {

void on_back(lv_event_t *e) {
  switch_panel<MainPanel>();
}

void on_rough_leveling(lv_event_t *e) {
  switch_panel<RoughPanel>();
}

#if HAS_LEVELING

void on_automatic_leveling(lv_event_t *e) {
  switch_panel<AutomaticPanel>();
}

void on_reset_leveling(lv_event_t *e) {
  BLTouch::_reset();
}

void on_z_height(lv_event_t *e) {
  switch_panel<ZHeightPanel>();
}

void on_leveling_grid(lv_event_t *e) {
  switch_panel<GridPanel>();
}

#endif

}
