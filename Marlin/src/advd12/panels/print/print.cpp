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
#include "../../core/ids.h"

namespace {

Panel<8> panel;

void on_back(lv_event_t *e);
void on_stop(lv_event_t *e);
void on_pause(lv_event_t *e);
void on_change(lv_event_t *e);
void on_options(lv_event_t *e);
void on_down(lv_event_t *e);
void on_up(lv_event_t *e);
void on_increment_selected(lv_event_t *e);

}

bool PrintPanel::can_switch() {
 return true;
}

void PrintPanel::create() {
 if(!panel.create()) return;
 panel.layout_menu("");

 panel.rectangle(0, 103, 342, 3);

 auto back = panel.control(IMG_BACK, 7, 2);
 auto stop = panel.control(IMG_STOP, 77, 2);
 auto pause = panel.control(IMG_PAUSE, 147, 2);
 auto change = panel.control(IMG_CHANGE, 217, 2);
 auto options = panel.control(IMG_OPTIONS, 289, 2);

 panel.icon(IMG_STEPS2, 8, 58);
 auto down = panel.small_control(IMG_DOWN, 77, 60);
 auto up = panel.small_control(IMG_UP, 194, 60);
 auto baby_steps = panel.text("0.00", 115, 68, 87, ALIGN::CENTER, LABEL::REGULAR);

 auto drop = lv_dropdown_create(panel.scr());
 lv_dropdown_set_options(drop, "0.02\n0.08\n0.16");
 lv_obj_set_pos(drop, 249, 60);
 lv_obj_set_size(drop, 87, 38);
 lv_obj_add_style(drop, &style_dropdown, LV_PART_MAIN);
 auto drop_list = lv_dropdown_get_list(drop);
 if(drop_list) lv_obj_add_style(drop_list, &style_dropdown, LV_PART_MAIN);

 panel.reset_bindings();
 panel.bind(back, &on_back);
 panel.bind(stop, &on_stop);
 panel.bind(pause, &on_pause);
 panel.bind(change, &on_change);
 panel.bind(options, &on_options);
 panel.bind(down, &on_down);
 panel.bind(up, &on_up);
 panel.bind(drop, &on_increment_selected);
}

namespace {

void on_back(lv_event_t *e) {
 PanelBase::switch_panel<MainPanel>();
}

void on_stop(lv_event_t *e) {
  if(!ExtUI::isPrinting()) return;

  Wait::wait_back_continue(GET_TEXT(ADVD12_TITLE_CONFIRM), GET_TEXT(ADVD12_MSG_ABORT_PRINT), [] (CALLBACK_SOURCE src) -> void {
   if(src == CALLBACK_SOURCE::BACK) { // Cancel abort print
     Status::set(GET_TEXT(ADVD12_MSG_PRINT_CONTINUE), Status::STATUS_OPTIONS::RESET);
     return;
   }

   ExtUI::stopPrint();
  });
}

void on_pause(lv_event_t *e) {
  if(!ExtUI::isPrinting()) return;
  if(ExtUI::isPrintingPaused())
    ExtUI::resumePrint();
  else
    ExtUI::pausePrint();
}

void on_change(lv_event_t *e) {
  if(!ExtUI::isPrinting()) return;
  ExtUI::injectCommands("M600");
}

void on_options(lv_event_t *e) {

}

void on_down(lv_event_t *e) {

}

void on_up(lv_event_t *e) {

}

void on_increment_selected(lv_event_t *e) {

}

}
