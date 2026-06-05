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

#include "panels.h"
#include "../core/styles.h"
#include "../core/ids.h"
#include "../core/core.h"
#include "../core/display.h"

PanelBase *PanelBase::current_panel_ = nullptr;

void PanelBase::release() {
  if(!scr_) return;
  lv_obj_del(scr_);
  scr_ = nullptr;
}

bool PanelBase::create() {
  if(scr_ != nullptr) return false;
  scr_ = lv_obj_create(nullptr);
  lv_obj_add_style(scr_, &style_panel, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_scr_load(scr_);

  if(current_panel_) current_panel_->release();
  current_panel_ = this;
  return true;
}

lv_obj_t* PanelBase::rectangle(lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height) {
  auto rect = lv_obj_create(scr_);
  lv_obj_set_pos(rect, x, y);
  lv_obj_set_size(rect, width, height);
  lv_obj_add_style(rect, &style_rectangle, LV_PART_MAIN | LV_STATE_DEFAULT);
  return rect;
}

lv_obj_t* PanelBase::button(const char *img, const char *text, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, lv_coord_t label_y_offset) {
  lv_obj_t * btn = lv_imgbtn_create(scr_);
  lv_obj_set_pos(btn, x, y);
  lv_obj_set_width(btn, w);
  lv_obj_set_height(btn, h);

  lv_imgbtn_set_src(btn, LV_IMGBTN_STATE_RELEASED, nullptr, img, nullptr);
  lv_imgbtn_set_src(btn, LV_IMGBTN_STATE_PRESSED,  nullptr, img, nullptr);
  lv_imgbtn_set_src(btn, LV_IMGBTN_STATE_DISABLED, nullptr, img, nullptr);

  lv_obj_add_style(btn, &style_button_up, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(btn, &style_button_down, LV_PART_MAIN | LV_STATE_PRESSED);

  lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE);

  if(text) {
    auto label = lv_label_create(scr_);
    lv_label_set_text(label, text);
    lv_obj_add_style(label, &style_button_label, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align_to(label, btn, LV_ALIGN_BOTTOM_MID, 0, label_y_offset);
  }

  return btn;
}

lv_obj_t* PanelBase::big_button(const char *img, const char *text, lv_coord_t x, lv_coord_t y) {
  return button(img, text, x, y, 64, 64, 32);
}

lv_obj_t* PanelBase::medium_button(const char *img, const char *text, lv_coord_t x, lv_coord_t y) {
  return button(img, text, x, y, 54, 54, 20);
}

lv_obj_t* PanelBase::control(const char *img, lv_coord_t x, lv_coord_t y, const char *text) {
  auto btn = button(img, nullptr, x, y, 48, 48, 0);

  if(text) {
    auto label = lv_label_create(scr_);
    lv_label_set_text(label, text);
    lv_obj_add_style(label, &style_button_label, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align_to(label, btn, LV_ALIGN_BOTTOM_MID, 0, 20);
  }

  return btn;
}

lv_obj_t* PanelBase::small_control(const char *img, lv_coord_t x, lv_coord_t y) {
  return button(img, nullptr, x, y, 38, 38, 0);
}

lv_obj_t* PanelBase::icon(const char *img, lv_coord_t x, lv_coord_t y) {
  auto icon = lv_img_create(scr_);
  lv_img_set_src(icon, img);
  lv_obj_set_pos(icon, x, y);
  return icon;
}

static lv_style_t *label_style(LABEL kind) {
  switch(kind) {
    case LABEL::EXTRUDER: return &style_label_extruder;
    case LABEL::BED: return &style_label_bed;
    case LABEL::FAN: return &style_label_fan;
    case LABEL::TIME: return &style_label_time;
    default: return &style_label_regular;
  }
}

lv_obj_t* PanelBase::text(const char *text, lv_coord_t x, lv_coord_t y, LABEL kind) {
  auto label = lv_label_create(scr_);
  lv_label_set_text(label, text);
  lv_obj_set_pos(label, x, y);
  lv_obj_add_style(label, label_style(kind), LV_PART_MAIN | LV_STATE_DEFAULT);
  return label;
}

static lv_text_align_t label_align(ALIGN align) {
  switch(align) {
    case ALIGN::LEFT: return LV_TEXT_ALIGN_LEFT;
    case ALIGN::RIGHT: return LV_TEXT_ALIGN_RIGHT;
    case ALIGN::CENTER: return LV_TEXT_ALIGN_CENTER;
    default: return LV_TEXT_ALIGN_LEFT;
  }
}

lv_obj_t* PanelBase::text(const char *text, lv_coord_t x, lv_coord_t y, lv_coord_t width, ALIGN align, LABEL kind) {
  auto label = lv_label_create(scr_);
  lv_label_set_text(label, text);
  lv_obj_set_pos(label, x, y);
  lv_obj_add_style(label, label_style(kind), LV_PART_MAIN);
  lv_label_set_long_mode(label, LV_LABEL_LONG_CLIP);
  lv_obj_set_width(label, width);
  lv_obj_set_style_text_align(label, label_align(align), LV_PART_MAIN);
  return label;
}

void PanelBase::layout_main() {
  rectangle(344, 0, 3, 284);
  rectangle(0, 284, 480, 3);
}

void PanelBase::layout_menu(const char *title) {
  rectangle(344, 0, 3, 284);
  rectangle(0, 284, 480, 3);
  auto rect = rectangle(0, 53, 344, 3);

  auto label = lv_label_create(scr_);
  lv_label_set_text(label, title);
  lv_obj_align_to(label, rect, LV_ALIGN_TOP_MID, 0, -36);
  lv_obj_add_style(label, &style_label_regular, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void PanelBase::variables_time() {
  icon(IMG_EXTRUDER, 363, 19);
  text("198.0", 398, 11, 64, ALIGN::RIGHT, LABEL::EXTRUDER);
  text("200", 398, 31, 64, ALIGN::RIGHT, LABEL::EXTRUDER);

  icon(IMG_BED, 363, 73);
  text("100.6", 398, 65, 64, ALIGN::RIGHT, LABEL::BED);
  text("100", 398, 85, 64, ALIGN::RIGHT, LABEL::BED);

  icon(IMG_FAN, 363, 118);
  text("100", 398, 118 + 4, 64, ALIGN::RIGHT, LABEL::FAN);

  icon(IMG_ET, 363, 179);
  text("00:32", 398, 179 + 2, 64, ALIGN::RIGHT, LABEL::TIME);
  icon(IMG_TC, 363, 211);
  text("12:34", 398, 211 + 2, 64, ALIGN::RIGHT, LABEL::TIME);
  icon(IMG_PERCENT, 363, 243);
  text("40", 398, 243 + 2, 64, ALIGN::RIGHT, LABEL::TIME);
}

void PanelBase::variables_xyz() {
  // TODO
}
