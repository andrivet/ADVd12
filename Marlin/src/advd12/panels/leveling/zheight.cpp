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
#include "../../../lcd/extui/ui_api.h"
#if HAS_LEVELING
#include <lvgl.h>
#include "zheight.h"
#include "leveling.h"
#include "../panels.h"
#include "../wait.h"
#include "../../core/ids.h"
#include "../../core/styles.h"
#include "src/advd12/core/format.h"
#include "src/advd12/core/status.h"

namespace {
constexpr xyz_feedrate_t homing_feedrate_mm_m = HOMING_FEEDRATE_MM_M;
constexpr float FEEDRATE_XY = MMM_TO_MMS(homing_feedrate_mm_m.x);
constexpr float FEEDRATE_Z = MMM_TO_MMS(homing_feedrate_mm_m.z);

Panel<5> panel;
float increment = 0.02;
float old_offset = 0;
lv_obj_t *z_offset = nullptr;

void prepare();
void on_back(lv_event_t *e);
void on_save(lv_event_t *e);
void on_up(lv_event_t *e);
void on_down(lv_event_t *e);
void on_increment(lv_event_t *e);
}

bool ZHeightPanel::can_switch() {
  return !ExtUI::isPrinting();
}

void ZHeightPanel::create() {
  if(!panel.create()) return;
  panel.layout_menu("Z Height");

  auto back = panel.control(IMG_LEFT, 7, 2);
  auto save = panel.control(IMG_SAVE, 289, 2);
  auto down = panel.control(IMG_DOWN, 32, 70);
  auto up = panel.control(IMG_UP, 159, 70);

  z_offset = panel.text("0.00", 80, 84, 79, ALIGN::CENTER, LABEL::REGULAR);

  auto dropdown = lv_dropdown_create(panel.scr());
  lv_dropdown_set_options(dropdown, "0.02\n0.10\n1.00");
  lv_obj_set_pos(dropdown, 223, 75);
  lv_obj_set_size(dropdown, 87, 38);
  lv_obj_add_style(dropdown, &style_dropdown, LV_PART_MAIN);
  auto drop_list = lv_dropdown_get_list(dropdown);
  if(drop_list) lv_obj_add_style(drop_list, &style_dropdown, LV_PART_MAIN);

  static const char text[] = "Adjust the height so a feeler gauge can move between the nozzle and the bed with just a tiny bit of friction.";
  auto label = lv_label_create(panel.scr());
  lv_label_set_text(label, text);
  lv_obj_set_pos(label, 22, 137);
  lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
  lv_obj_set_width(label, 293);
  lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

  panel.reset_bindings();
  panel.bind(back, &on_back);
  panel.bind(save, &on_save);
  panel.bind(down, &on_down);
  panel.bind(up, &on_up);
  panel.bind(dropdown, &on_increment, EventType::CHANGED);

  prepare();
}

namespace {

void on_back(lv_event_t *e) {
  ExtUI::setTargetTemp_celsius(0, ExtUI::H0);
  ExtUI::setSoftEndstopState(true);
  ExtUI::setZOffset_mm(old_offset);
  ExtUI::setAbsoluteZAxisPosition_mm(ExtUI::getAxisPosition_mm(ExtUI::Z) - old_offset);
  ExtUI::setAxisPosition_mm(Z_AFTER_HOMING, ExtUI::Z, FEEDRATE_Z);
  Wait::not_busy([](CALLBACK_SOURCE) { switch_panel<LevelingPanel>(); });
}

void on_save(lv_event_t *e) {
  ExtUI::setTargetTemp_celsius(0, ExtUI::H0);
  // Current Z position becomes Z offset
  ExtUI::setSoftEndstopState(true);
  ExtUI::setZOffset_mm(ExtUI::getAxisPosition_mm(ExtUI::Z));
  ExtUI::setAbsoluteZAxisPosition_mm(0);
  ExtUI::setAxisPosition_mm(Z_AFTER_HOMING, ExtUI::Z, FEEDRATE_Z);
  ExtUI::saveSettings();
  LCD_MESSAGE(ADVD12_MSG_Z_OFFSET_SAVED);
  Wait::not_busy([](CALLBACK_SOURCE) { switch_panel<LevelingPanel>(); });
}

void set_offset() {
  char text[8];
  Format::format00(text, sizeof text, ExtUI::getAxisPosition_mm(ExtUI::Z));
  lv_label_set_text(z_offset, text);
}

void adjust_height(float offset_increment) {
  ExtUI::setAxisPosition_mm(ExtUI::getAxisPosition_mm(ExtUI::Z) + offset_increment, ExtUI::Z, FEEDRATE_Z);
  set_offset();
}

void on_up(lv_event_t *e) {
  adjust_height(increment);
}

void on_down(lv_event_t *e) {
  adjust_height(-increment);
}

float get_multiplier(uint16_t index) {
  switch(index) {
    case 0: return 0.02;
    case 1: return 0.10;
    case 2: return 1.00;
    default: return 0.02;
  }
}

void on_increment(lv_event_t *e) {
  increment = get_multiplier(lv_dropdown_get_selected(e->current_target));
}

void prepare() {
  old_offset = ExtUI::getZOffset_mm();
  ExtUI::setZOffset_mm(0); // Before homing otherwise, Marlin is lost
  ExtUI::setAbsoluteZAxisPosition_mm(ExtUI::getAxisPosition_mm(ExtUI::Z) + old_offset);

  Wait::homing(nullptr, []() -> void {
    float positions[2] = {X_CENTER, Y_CENTER};
    ExtUI::axis_t axis[2] = { ExtUI::X, ExtUI::Y };
    ExtUI::setMultipleAxisPosition_mm(2, positions, axis, FEEDRATE_XY);
    ExtUI::setAxisPosition_mm(0, ExtUI::Z, FEEDRATE_Z);
    ExtUI::setSoftEndstopState(false);
    set_offset();
  });
}

}
#endif
