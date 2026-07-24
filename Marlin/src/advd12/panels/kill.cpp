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
#include "../../inc/MarlinConfig.h"
#include "kill.h"
#include "wait.h"

namespace {
void text(lv_obj_t *parent, const char* text, lv_color_t color, lv_coord_t y);
}


void Kill::killed(const char *msg, const char *component) {
  auto red = lv_color_make(255, 138, 97);
  auto parent = lv_scr_act();

  Wait::close();

  auto modal = lv_obj_create(parent);
  lv_obj_clear_flag(modal, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_size(modal, LV_PCT(100), LV_PCT(100));
  lv_obj_center(modal);
  lv_obj_add_flag(modal, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_style_bg_color(modal, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(modal, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);

  auto msgbox = lv_obj_create(parent);
  lv_obj_clear_flag(msgbox, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_size(msgbox, 329, 233);
  lv_obj_set_pos(msgbox, 7, 40);
  lv_obj_set_style_bg_color(msgbox, red, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(msgbox, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(msgbox, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  auto line = lv_obj_create(msgbox);
  lv_obj_set_pos(line, 0, 55);
  lv_obj_set_size(line, 329, 3);
  lv_obj_set_style_bg_color(line, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(line, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

  auto title = lv_label_create(msgbox);
  lv_label_set_text(title, "ERROR");
  lv_obj_set_style_text_color(title, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(title, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_align_to(title, line, LV_ALIGN_BOTTOM_MID, 0, -18);

  text(msgbox, "A hardware error has been detected:", lv_color_black(), 72);
  text(msgbox, msg, lv_color_white(), 102);
  if(component) text(msgbox, component, lv_color_white(), 102 + 22);
  text(msgbox, "The printer has stopped to prevent causing damage.", lv_color_black(), 160);

  lv_refr_now(nullptr);
}

void Kill::killed(const char *msg, heater_id_t header_id) {
  if(header_id == H_BED)
    killed(msg, "Bed");
  else if(header_id == H_E0)
    killed(msg, "Extruder");
  else
    killed(msg, nullptr);
}

namespace {

void text(lv_obj_t *parent, const char* text, lv_color_t color, lv_coord_t y) {
  auto label = lv_label_create(parent);
  lv_obj_set_style_text_color(label, color, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(label, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_width(label, lv_pct(100));
  lv_label_set_text(label, text);
  lv_obj_set_pos(label, 0, y);
}

}
