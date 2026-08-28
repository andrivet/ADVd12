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
#include "../../../inc/MarlinConfig.h"
#include "../../../lcd/extui/ui_api.h"
#include "rough.h"
#include "bed.h"
#include "leveling.h"
#include "../panels.h"
#include "../wait.h"
#include "../../core/ids.h"
#include "../../core/status.h"

namespace {

constexpr unsigned MARGIN = 30;
constexpr xyz_feedrate_t homing_feedrate_mm_m = HOMING_FEEDRATE_MM_M;
constexpr float FEEDRATE_XY = MMM_TO_MMS(homing_feedrate_mm_m.x);
constexpr float FEEDRATE_Z = MMM_TO_MMS(homing_feedrate_mm_m.z);

Panel<10> panel;

void prepare(CALLBACK_SOURCE src);

void on_back(lv_event_t *e);
void on_1(lv_event_t *e);
void on_2(lv_event_t *e);
void on_3(lv_event_t *e);
void on_4(lv_event_t *e);
void on_5(lv_event_t *e);
}

bool RoughPanel::can_switch() {
  return Wait::check_not_busy();
}

void RoughPanel::create() {
  if(!panel.create()) return;
  panel.layout_menu("Rough");
  Bed::create(panel.scr());

  auto back = panel.control(IMG_LEFT, 7, 2);
  auto level_1 = panel.circle_button(46, 171, "1");
  auto level_2 = panel.circle_button(250, 171, "2");
  auto level_3 = panel.circle_button(226, 78, "3");
  auto level_4 = panel.circle_button(70, 78, "4");
  auto level_5 = panel.circle_button(148, 125, "5");

  panel.reset_bindings();
  panel.bind(back, on_back);
  panel.bind(level_1, on_1);
  panel.bind(level_2, on_2);
  panel.bind(level_3, on_3);
  panel.bind(level_4, on_4);
  panel.bind(level_5, on_5);

  if(ExtUI::getActualTemp_celsius(ExtUI::E0) > 50)
    Wait::wait_cancel_continue(GET_TEXT(ADVD12_TITLE_WARNING), GET_TEXT(ADVD12_MSG_LEVELING_HOT), &prepare);
  else
    prepare(CALLBACK_SOURCE::CONTINUE);
}

namespace {

void prepare(CALLBACK_SOURCE src) {
  if(src == CALLBACK_SOURCE::BACK) {
    Status::set(GET_TEXT(ADVD12_MSG_CANCELED), Status::STATUS_OPTIONS::RESET);
    switch_panel<LevelingPanel>();
    return;
  }

#if HAS_LEVELING
  ExtUI::setLevelingActive(false); // We do not want compensation during manual leveling
#endif

  Wait::homing(F("G28\nG1 Z4 F1200"), [] () -> void {
    Wait::not_busy([](CALLBACK_SOURCE) { switch_panel<RoughPanel>(); });
  });
}

void on_back(lv_event_t *e) {
#if HAS_LEVELING
  ExtUI::setLevelingActive(true); // Enable back compensation
#endif
  ExtUI::setAxisPosition_mm(Z_AFTER_HOMING, ExtUI::Z, FEEDRATE_Z);
  Wait::not_busy([](CALLBACK_SOURCE) { switch_panel<LevelingPanel>(); } );
}

void move(float x, float y) {
  ExtUI::setAxisPosition_mm(Z_CLEARANCE_FOR_HOMING, ExtUI::Z, FEEDRATE_Z);

  float positions[2] = {x, y};
  ExtUI::axis_t axis[2] = { ExtUI::X, ExtUI::Y };
  ExtUI::setMultipleAxisPosition_mm(2, positions, axis, FEEDRATE_XY);
  ExtUI::setAxisPosition_mm(0, ExtUI::Z, FEEDRATE_Z);
}

void on_1(lv_event_t *e) {
  move(MARGIN, MARGIN);
}

void on_2(lv_event_t *e) {
  move(X_BED_SIZE - MARGIN, MARGIN);
}

void on_3(lv_event_t *e) {
  move(X_BED_SIZE - MARGIN, Y_BED_SIZE - MARGIN);
}

void on_4(lv_event_t *e) {
  move(MARGIN, Y_BED_SIZE - MARGIN);
}

void on_5(lv_event_t *e) {
  move(X_BED_SIZE / 2., Y_BED_SIZE / 2.);
}


}
