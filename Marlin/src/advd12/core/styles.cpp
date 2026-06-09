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

#include "styles.h"
#include "display.h"

void Display::init_lvgl_styles() {
  auto value_font = &lv_font_montserrat_20;
  auto label_font = &lv_font_montserrat_16;

  lv_style_init(&style_panel);
  lv_style_set_bg_color(&style_panel, lv_color_black());
  lv_style_set_bg_opa(&style_panel, LV_OPA_COVER);

  lv_style_init(&style_rectangle);
  lv_style_set_bg_color(&style_rectangle, lv_color_make(255, 148, 0));
  lv_style_set_bg_opa(&style_rectangle, LV_OPA_COVER);

  lv_style_init(&style_button_down);
  lv_style_set_img_recolor(&style_button_down, lv_color_black());
  lv_style_set_img_recolor_opa(&style_button_down, LV_OPA_40);

  lv_style_init(&style_button_up);

  lv_style_init(&style_button_label);
  lv_style_set_text_color(&style_button_label, lv_color_white());
  lv_style_set_text_font(&style_button_label, label_font);

  lv_style_init(&style_label_regular);
  lv_style_set_text_color(&style_label_regular, lv_color_white());
  lv_style_set_text_font(&style_label_regular, value_font);

  lv_style_init(&style_label_extruder);
  lv_style_set_text_color(&style_label_extruder, lv_color_make(255, 138, 97));
  lv_style_set_text_font(&style_label_extruder, value_font);

  lv_style_init(&style_label_bed);
  lv_style_set_text_color(&style_label_bed, lv_color_make(126, 211, 33));
  lv_style_set_text_font(&style_label_bed, value_font);

  lv_style_init(&style_label_fan);
  lv_style_set_text_color(&style_label_fan, lv_color_make(116, 198, 255));
  lv_style_set_text_font(&style_label_fan, value_font);

  lv_style_init(&style_label_time);
  lv_style_set_text_color(&style_label_time, lv_color_make(151, 151, 151));
  lv_style_set_text_font(&style_label_time, value_font);
}
