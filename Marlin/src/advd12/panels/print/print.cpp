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
#include "print.h"
#include "../panels.h"
#include "../main.h"
#include "../wait.h"
#include "../../core/styles.h"
#include "../../core/status.h"
#include "../../core/format.h"
#include "../../core/ids.h"

namespace {

char filename[64];
lv_obj_t *dropdown = nullptr;
lv_obj_t *z_offset_var = nullptr;
lv_obj_t *feedrate_var = nullptr;
lv_obj_t *flowrate_var = nullptr;
float increment = 0.01;

Panel<11> panel;

void display_z_offset();
void display_feedrate();
void display_flowrate();
float get_multiplier(uint16_t index);

void on_stop(lv_event_t *e);
void on_pause(lv_event_t *e);
void on_continue(lv_event_t *e);
//void on_change(lv_event_t *e);
void on_down(lv_event_t *e);
void on_up(lv_event_t *e);
void on_feedrate_minus(lv_event_t *e);
void on_feedrate_plus(lv_event_t *e);
void on_flowrate_minus(lv_event_t *e);
void on_flowrate_plus(lv_event_t *e);
void on_increment_selected(lv_event_t *e);

}

void PrintPanel::set_filename(const char *name) {
  auto length = strlen(name);
  if(length >= sizeof(filename))
    length = sizeof(filename) - 1;
  memcpy(filename, name, length);
}

bool PrintPanel::can_switch() {
 return true;
}

void PrintPanel::create() {
  panel.create();
  panel.layout_menu("");

  auto stop = panel.control(IMG_STOP, 7, 2);
  auto pause = panel.control(IMG_PAUSE, 74, 2);
  auto cont = panel.control(IMG_START, 140, 2);
  //auto change = panel.control(IMG_CHANGE, 288, 2);

  panel.icon(IMG_FS_FILE, 15, 64);
  panel.text(filename, 62, 69, 274, ALIGN::LEFT, LABEL::REGULAR);

  panel.icon(IMG_STEPS2, 8, 106);
  auto down = panel.control(IMG_DOWN, 62, 104);
  auto up = panel.control(IMG_UP, 189, 104);
  z_offset_var = panel.text("0.00", 110, 117, 79, ALIGN::CENTER, LABEL::REGULAR);

  dropdown = lv_dropdown_create(panel.scr());
  lv_dropdown_set_options(dropdown, "0.01\n0.05\n0.25");
  lv_obj_set_pos(dropdown, 249, 108);
  lv_obj_set_size(dropdown, 87, 38);
  lv_obj_add_style(dropdown, &style_dropdown, LV_PART_MAIN);
  auto drop_list = lv_dropdown_get_list(dropdown);
  if(drop_list) lv_obj_add_style(drop_list, &style_dropdown, LV_PART_MAIN);

  panel.icon(IMG_FEEDRATE2, 15, 172);
  auto feedrate_minus = panel.control(IMG_MINUS, 62, 162);
  auto feedrate_plus = panel.control(IMG_PLUS, 189, 162);
  feedrate_var = panel.text("100%", 110, 175, 79, ALIGN::CENTER, LABEL::REGULAR);

  panel.icon(IMG_FLOWRATE, 15, 228);
  auto flowrate_minus = panel.control(IMG_MINUS, 62, 220);
  auto flowrate_plus = panel.control(IMG_PLUS, 189, 220);
  flowrate_var = panel.text("100%", 110, 233, 79, ALIGN::CENTER, LABEL::REGULAR);

  panel.reset_bindings();
  panel.bind(stop, &on_stop);
  panel.bind(pause, &on_pause);
  panel.bind(cont, &on_continue);
  //panel.bind(change, &on_change);
  panel.bind(down, &on_down);
  panel.bind(up, &on_up);
  panel.bind(dropdown, &on_increment_selected, EventType::CHANGED);
  panel.bind(feedrate_minus, &on_feedrate_minus);
  panel.bind(feedrate_plus, &on_feedrate_plus);
  panel.bind(flowrate_minus, &on_flowrate_minus);
  panel.bind(flowrate_plus, &on_flowrate_plus);

  display_z_offset();

  Wait::heating([](CALLBACK_SOURCE src) {
    if(src == CALLBACK_SOURCE::BACK) {
      ExtUI::stopPrint();
      switch_panel<MainPanel>();
    }
  });
}

namespace {

void on_stop(lv_event_t *e) {
  Wait::wait_back_continue(GET_TEXT(ADVD12_TITLE_CONFIRM), GET_TEXT(ADVD12_MSG_ABORT_PRINT), [] (CALLBACK_SOURCE src) -> void {
    if(src == CALLBACK_SOURCE::BACK) { // Cancel abort print
      Status::set(GET_TEXT(ADVD12_MSG_PRINT_CONTINUE), Status::STATUS_OPTIONS::RESET);
      return;
    }

    ExtUI::stopPrint();
    switch_panel<MainPanel>();
  });
}

void on_pause(lv_event_t *e) {
  if(!ExtUI::isPrinting() || ExtUI::isPrintingPaused()) return;
  ExtUI::pausePrint();
}

void on_continue(lv_event_t *e) {
  if(!ExtUI::isPrinting() || !ExtUI::isPrintingPaused()) return;
  ExtUI::resumePrint();
}

/*
void on_change(lv_event_t *e) {
  if(!ExtUI::isPrinting()) return;
  ExtUI::injectCommands("M600");
}*/

void on_down(lv_event_t *e) {
  auto steps = ExtUI::mmToWholeSteps(increment, ExtUI::Z);
  ExtUI::smartAdjustAxis_steps(static_cast<int16_t>(-steps), ExtUI::Z, true);
  display_z_offset();
}

void on_up(lv_event_t *e) {
  auto steps = ExtUI::mmToWholeSteps(increment, ExtUI::Z);
  ExtUI::smartAdjustAxis_steps(steps, ExtUI::Z, true);
  display_z_offset();
}

void on_increment_selected(lv_event_t *e) {
  auto index = lv_dropdown_get_selected(dropdown);
  increment = get_multiplier(index);
}

float get_multiplier(uint16_t index) {
  switch(index) {
    case 0: return 0.01;
    case 1: return 0.05;
    case 2: return 0.25;
    default: return 0.01;
  }
}

void display_z_offset() {
  char text[8];
#if HAS_BED_PROBE
  auto offset = ExtUI::getZOffset_mm();
#else
  auto offset = ExtUI::getAxisPosition_mm(ExtUI::Z);
#endif
  Format::format00(text, sizeof text, offset);
  lv_label_set_text(z_offset_var, text);
}

void display_feedrate() {
  char text[8];
  sprintf(text, "%d%%", static_cast<int>(ExtUI::getFeedrate_percent()));
  lv_label_set_text(feedrate_var, text);
}

void display_flowrate() {
  char text[8];
  sprintf(text, "%d%%", static_cast<int>(ExtUI::getFlow_percent(ExtUI::E0)));
  lv_label_set_text(flowrate_var, text);
}

void on_feedrate_minus(lv_event_t *e) {
  auto feedrate = ExtUI::getFeedrate_percent();
  if(feedrate <= 50) return;
  ExtUI::setFeedrate_percent(feedrate - 1);
  display_feedrate();
}

void on_feedrate_plus(lv_event_t *e) {
  auto feedrate = ExtUI::getFeedrate_percent();
  if(feedrate >= 150) return;
  ExtUI::setFeedrate_percent(feedrate + 1);
  display_feedrate();
}

void on_flowrate_minus(lv_event_t *e) {
  auto flowrate = ExtUI::getFlow_percent(ExtUI::E0);
  if(flowrate <= 50) return;
  ExtUI::setFlow_percent(flowrate - 1, ExtUI::E0);
  display_flowrate();
}

void on_flowrate_plus(lv_event_t *e) {
  auto flowrate = ExtUI::getFlow_percent(ExtUI::E0);
  if(flowrate >= 150) return;
  ExtUI::setFlow_percent(flowrate + 1, ExtUI::E0);
  display_flowrate();
}

}
