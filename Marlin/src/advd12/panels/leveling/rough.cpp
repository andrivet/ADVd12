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
#include "../panels.h"
#include "../main.h"
#include "../wait.h"
#include "../../core/ids.h"
#include "../../core/styles.h"
#include "../../core/status.h"
#include "src/advd12/core/core.h"

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
void on_a(lv_event_t *e);
void on_b(lv_event_t *e);
void on_c(lv_event_t *e);
void on_d(lv_event_t *e);

}

bool RoughPanel::can_switch() {
  return Wait::check_not_busy();
}

void RoughPanel::create() {
  if(!panel.create()) return;
  panel.layout_menu("Rough");

  auto back = panel.control(IMG_BACK, 7, 2);
  auto level_1 = panel.circle_button(19, 199, "1");
  auto level_2 = panel.circle_button(135, 199, "2");
  auto level_3 = panel.circle_button(135, 83, "3");
  auto level_4 = panel.circle_button(19, 83, "4");
  auto level_5 = panel.circle_button(77, 141, "5");
  auto level_a = panel.circle_button(77, 199, "A");
  auto level_b = panel.circle_button(19, 141, "B");
  auto level_c = panel.circle_button(77, 83, "C");
  auto level_d = panel.circle_button(135, 141, "D");

  panel.reset_bindings();
  panel.bind(back, on_back);
  panel.bind(level_1, on_1);
  panel.bind(level_2, on_2);
  panel.bind(level_3, on_3);
  panel.bind(level_4, on_4);
  panel.bind(level_5, on_5);
  panel.bind(level_a, on_a);
  panel.bind(level_b, on_b);
  panel.bind(level_c, on_c);
  panel.bind(level_d, on_d);

  auto label = lv_label_create(panel.scr());
  lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
  lv_label_set_text(label, "Press the buttons, adjust the thumb nuts and use a feeler gauge. It should move with a tiny bit of friction.");
  lv_obj_add_style(label, &style_button_label, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_pos(label, 193, 83);
  lv_obj_set_width(label, 134);
  lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);

  if(ExtUI::getActualTemp_celsius(ExtUI::E0) > 50)
    Wait::wait_back_continue(GET_TEXT(ADVD12_TITLE_WARNING), GET_TEXT(ADVD12_MSG_LEVELING_HOT), &prepare);
  else
    prepare(CALLBACK_SOURCE::CONTINUE);
}

namespace {

void prepare(CALLBACK_SOURCE src) {
  if(src == CALLBACK_SOURCE::BACK) {
    ExtUI::setTargetTemp_celsius(0, ExtUI::BED);
    Status::set(GET_TEXT(ADVD12_MSG_CANCELED), Status::STATUS_OPTIONS::RESET);
    PanelBase::switch_panel<MainPanel>();
    return;
  }

#if HAS_LEVELING
  ExtUI::setLevelingActive(false); // We do not want compensation during manual leveling
#endif

  Wait::homing(F("G28\nG1 Z4 F1200"), [] () -> void {
    Wait::not_busy([](CALLBACK_SOURCE) { PanelBase::switch_panel<RoughPanel>(); });
  });
}

void on_back(lv_event_t *e) {
#if HAS_LEVELING
  ExtUI::setLevelingActive(true); // Enable back compensation
#endif
  ExtUI::setAxisPosition_mm(Z_AFTER_HOMING, ExtUI::Z, FEEDRATE_Z);
  Wait::not_busy([](CALLBACK_SOURCE) { PanelBase::switch_panel<MainPanel>(); } );
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

void on_a(lv_event_t *e) {
  move(X_BED_SIZE / 2., 30);
}

void on_b(lv_event_t *e) {
  move(MARGIN, X_BED_SIZE / 2.);
}

void on_c(lv_event_t *e) {
  move(X_BED_SIZE / 2., Y_BED_SIZE - MARGIN);
}

void on_d(lv_event_t *e) {
  move(X_BED_SIZE - MARGIN, X_BED_SIZE / 2.);
}


}
