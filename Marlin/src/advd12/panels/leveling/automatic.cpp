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
#include "automatic.h"
#include "bed.h"
#include "leveling.h"
#include "grid.h"
#include "../panels.h"
#include "../wait.h"
#include "../../core/ids.h"
#include "../../core/status.h"

namespace {

constexpr int16_t NB_POINTS = GRID_MAX_POINTS_Y * GRID_MAX_POINTS_X;

Panel<1> panel;
lv_obj_t *probe;

void prepare();

void on_back(lv_event_t *e);

}

bool AutomaticPanel::can_switch() {
  return true;
}

void AutomaticPanel::create() {
  if(!panel.create()) return;
  panel.layout_menu("Automatic");
  Bed::create(panel.scr());

  auto back = panel.control(IMG_LEFT, 7, 2);

  panel.reset_bindings();
  panel.bind(back, on_back);

  prepare();
}

void AutomaticPanel::on_leveling_start() {
  LCD_MESSAGE(MSG_BED_LEVELING);
  probe = panel.icon(IMG_PROBE, 60, 177);
}

void AutomaticPanel::on_leveling_progress(uint8_t progress) {
  char text[20];
  sprintf(text, GET_TEXT(ADVD12_MSG_LEVELING_PROBING), static_cast<int>(progress), NB_POINTS);
  MarlinUI::set_status(text);

  static const lv_point_t points[] = {
    {60, 177}, {164, 177}, {264, 177 },
    {258, 130}, {164, 130}, {70, 130},
    {81, 83}, {164, 83}, {247, 83}
  };

  auto &point = points[progress - 1];
  lv_obj_set_pos(probe, point.x, point.y);
}

void AutomaticPanel::on_leveling_done(bool success) {
  ExtUI::setTargetTemp_celsius(0, ExtUI::BED);

  if(!success)
    LCD_MESSAGE(ADVD12_MSG_LEVELING_FAILURE);
  else {
    ExtUI::saveSettings();
    ExtUI::setLevelingActive(true);
    switch_panel<GridPanel>();
  }
}

namespace {

void prepare() {
  Wait::homing(F("G28"), [] () -> void {
    switch_panel<AutomaticPanel>();
    ExtUI::injectCommands(ExtUI::isLevelingHighSpeed() ? F("G29\nG28 X Y") : F("G29 E\nG28 X Y"));
  });
}

void on_back(lv_event_t *e) {
  ExtUI::setTargetTemp_celsius(0, ExtUI::BED);
  LCD_MESSAGE(ADVD12_MSG_LEVELING_CANCEL);
  ExtUI::cancelLeveling();
  Wait::not_busy([](CALLBACK_SOURCE) { switch_panel<LevelingPanel>(); } );
}

}

#endif
