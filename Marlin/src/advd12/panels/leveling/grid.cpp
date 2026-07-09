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
#ifdef BLTOUCH
#include <lvgl.h>
#include "../../../inc/MarlinConfig.h"
#include "../../../lcd/extui/ui_api.h"
#include "grid.h"
#include "bed.h"
#include "leveling.h"
#include "../panels.h"
#include "../wait.h"
#include "../../core/ids.h"
#include "../../core/status.h"

namespace {

constexpr int16_t NB_POINTS = GRID_MAX_POINTS_Y * GRID_MAX_POINTS_X;

Panel<1> panel;

void grid();

void on_back(lv_event_t *e);

}

bool GridPanel::can_switch() {
  return true;
}

void GridPanel::create() {
  if(!panel.create()) return;
  panel.layout_menu("Grid");
  Bed::create(panel.scr());

  auto back = panel.control(IMG_LEFT, 7, 2);
  grid();

  panel.reset_bindings();
  panel.bind(back, on_back);
}

namespace {

void grid() {
  static const lv_point_t points[GRID_MAX_POINTS_Y][GRID_MAX_POINTS_X] = {
    {{52, 195},{155, 195}, {252, 195}},
    {{70, 138}, {155, 138}, {234, 138}},
    {{82, 82}, {155, 82}, {218, 82}}
  };
  static const ALIGN aligns[GRID_MAX_POINTS_X] = {
    ALIGN::LEFT, ALIGN::CENTER, ALIGN::LEFT
  };

  auto z_values = ExtUI::getMeshArray();
  char text[8];

  for(auto y = 0; y < GRID_MAX_POINTS_Y; y++)
    for(auto x = 0; x < GRID_MAX_POINTS_X; x++) {
      sprintf(text, "%.2f", z_values[x][y]);
      auto &point = points[y][x];
      panel.text(text, point.x, point.y, 48, aligns[x], LABEL::REGULAR);
    }

}

void on_back(lv_event_t *e) {
  PanelBase::switch_panel<LevelingPanel>();
}

}
#endif
