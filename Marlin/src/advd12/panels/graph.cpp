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
#include "../../lcd/extui/ui_api.h"
#include "graph.h"
#include "../core/styles.h"

namespace {

constexpr unsigned NB_POINTS = 240;
constexpr unsigned BED_TEMP_MAX = 155;
constexpr unsigned BED_TEMP_MIN = 25;
constexpr unsigned EXT_TEMP_MAX = 260;
constexpr unsigned EXT_TEMP_MIN = 40;

lv_obj_t *chart = nullptr;
lv_chart_series_t *bed_series = nullptr;
lv_chart_series_t *ext_series = nullptr;

void draw_graduations(lv_obj_t *parent);

}

void Graph::create(lv_obj_t *parent) {
  draw_graduations(parent);

  chart = lv_chart_create(parent);
  lv_obj_set_pos(chart, 37, 58);
  lv_obj_set_size(chart, NB_POINTS, 168);
  lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
  lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, BED_TEMP_MIN, BED_TEMP_MAX);
  lv_chart_set_range(chart, LV_CHART_AXIS_SECONDARY_Y, EXT_TEMP_MIN, EXT_TEMP_MAX);
  lv_chart_set_point_count(chart, NB_POINTS);
  lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);
  lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 0, 0, 0, false, 0);
  lv_chart_set_axis_tick(chart, LV_CHART_AXIS_SECONDARY_Y, 0, 0, 0, 0, false, 0);
  bed_series = lv_chart_add_series(chart, lv_color_make(126, 211, 33), LV_CHART_AXIS_PRIMARY_Y);
  ext_series = lv_chart_add_series(chart, lv_color_make(255, 138, 97), LV_CHART_AXIS_SECONDARY_Y);
  lv_chart_set_div_line_count(chart, 0, 0);
  lv_chart_set_all_value(chart, bed_series, static_cast<lv_coord_t>(ExtUI::getActualTemp_celsius(ExtUI::BED)));
  lv_chart_set_all_value(chart, ext_series, static_cast<lv_coord_t>(ExtUI::getActualTemp_celsius(ExtUI::E0)));

  lv_obj_set_style_border_width(chart, 0, LV_PART_MAIN);
  lv_obj_set_style_line_color(chart, lv_color_white(), LV_PART_ITEMS);
  lv_obj_set_style_line_width(chart, 2, LV_PART_ITEMS);
}

void Graph::next_point() {
  lv_chart_set_next_value(chart, ext_series, static_cast<lv_coord_t>(ExtUI::getActualTemp_celsius(ExtUI::E0)));
  lv_chart_set_next_value(chart, bed_series, static_cast<lv_coord_t>(ExtUI::getActualTemp_celsius(ExtUI::BED)));
}

namespace {

void draw_left_graduation(lv_obj_t *parent, const char *text, lv_coord_t y) {
  auto label = lv_label_create(parent);
  lv_label_set_text(label, text);
  lv_obj_set_pos(label, 7, y);

  lv_obj_set_style_text_color(label, green, LV_PART_MAIN);
  lv_obj_set_style_text_font(label, &lv_font_montserrat_20, LV_PART_MAIN);

  lv_label_set_long_mode(label, LV_LABEL_LONG_CLIP);
  lv_obj_set_width(label, 32);
  lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);
}

void draw_right_graduation(lv_obj_t *parent, const char *text, lv_coord_t y) {
  auto label = lv_label_create(parent);
  lv_label_set_text(label, text);
  lv_obj_set_pos(label, 280, y);

  lv_obj_set_style_text_color(label, red, LV_PART_MAIN);
  lv_obj_set_style_text_font(label, &lv_font_montserrat_20, LV_PART_MAIN);

  lv_label_set_long_mode(label, LV_LABEL_LONG_CLIP);
  lv_obj_set_width(label, 32);
  lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);
}

void draw_graduations(lv_obj_t *parent) {
  char text[8];
  for(unsigned i = 0; i < 5; i++) {
    sprintf(text, "%d", i * 30 + 30);
    draw_left_graduation(parent, text, 206 - i * 36);
    sprintf(text, "%d", i * 50 + 50);
    draw_right_graduation(parent, text, 206 - i * 36);

    auto rect = lv_obj_create(parent);
    lv_obj_set_pos(rect, 40, static_cast<lv_coord_t>(73 + i * 36));
    lv_obj_set_size(rect, NB_POINTS, 1);
    lv_obj_set_style_bg_color(rect, lv_color_make(128, 128, 128), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(rect, LV_OPA_COVER, LV_PART_MAIN);
  }
}

}
