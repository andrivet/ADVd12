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

#include <cstdio>
#include <lvgl.h>
#include "../../../inc/MarlinConfig.h"
#include "../../../lcd/extui/ui_api.h"
#include "../../core/ids.h"
#include "../../core/status.h"
#include "../panels.h"
#include "preheat.h"
#include "controls.h"

namespace {

unsigned index_ = 0;
lv_obj_t *index_text_ = nullptr;
lv_obj_t *ext_text_ = nullptr;
lv_obj_t *bed_text_ = nullptr;
lv_obj_t *fan_text_ = nullptr;

celsius_t ext_ = 0;
celsius_t bed_ = 0;
uint8_t fan_ = 0;

Panel<11> preheat_panel;

void on_back(lv_event_t *e);
void on_save(lv_event_t *e);
void on_previous(lv_event_t *e);
void on_next(lv_event_t *e);
void on_ext_minus(lv_event_t *e);
void on_ext_plus(lv_event_t *e);
void on_bed_minus(lv_event_t *e);
void on_bed_plus(lv_event_t *e);
void on_fan_minus(lv_event_t *e);
void on_fan_plus(lv_event_t *e);
void on_cooldown(lv_event_t *e);

void show();
void show_index();
void show_ext();
void show_bed();
void show_fan();
}

bool PreheatPanel::can_switch() {
  return true;
}

void PreheatPanel::create() {
  if(!preheat_panel.create()) return;
  preheat_panel.layout_menu("Preheat");

  auto back = preheat_panel.control(IMG_LEFT, 7, 2);
  auto save = preheat_panel.control(IMG_SAVE, 289, 2);

  preheat_panel.icon(IMG_EXTRUDER, 14, 133);
  preheat_panel.icon(IMG_BED, 14, 185);
  preheat_panel.icon(IMG_FAN, 14, 237);

  auto previous = preheat_panel.control(IMG_LEFT, 53, 64);
  auto next = preheat_panel.control(IMG_RIGHT, 173, 63);
  auto ext_minus = preheat_panel.control(IMG_MINUS, 53, 123);
  auto ext_plus = preheat_panel.control(IMG_PLUS, 173, 123);
  auto bed_minus = preheat_panel.control(IMG_MINUS, 53, 175);
  auto bed_plus = preheat_panel.control(IMG_PLUS, 173, 175);
  auto fan_minus = preheat_panel.control(IMG_MINUS, 53, 227);
  auto fan_plus = preheat_panel.control(IMG_PLUS, 173, 227);
  auto cooldown = preheat_panel.control(IMG_COOLDOWN, 262, 123, "Cool");

  preheat_panel.reset_bindings();
  preheat_panel.bind(back, &on_back);
  preheat_panel.bind(save, &on_save);
  preheat_panel.bind(previous, &on_previous);
  preheat_panel.bind(next, &on_next);
  preheat_panel.bind(ext_minus, &on_ext_minus, EventType::PRESSED | EventType::REPEAT);
  preheat_panel.bind(ext_plus, &on_ext_plus, EventType::PRESSED | EventType::REPEAT);
  preheat_panel.bind(bed_minus, &on_bed_minus, EventType::PRESSED | EventType::REPEAT);
  preheat_panel.bind(bed_plus, &on_bed_plus, EventType::PRESSED | EventType::REPEAT);
  preheat_panel.bind(fan_minus, &on_fan_minus, EventType::PRESSED | EventType::REPEAT);
  preheat_panel.bind(fan_plus, &on_fan_plus, EventType::PRESSED | EventType::REPEAT);
  preheat_panel.bind(cooldown, &on_cooldown);

  index_text_ = preheat_panel.text("", 101, 77, 72, ALIGN::CENTER, LABEL::REGULAR);
  ext_text_ = preheat_panel.text("", 101, 136, 72, ALIGN::CENTER, LABEL::REGULAR);
  bed_text_ = preheat_panel.text("", 101, 188, 72, ALIGN::CENTER, LABEL::REGULAR);
  fan_text_ = preheat_panel.text("", 101, 240, 72, ALIGN::CENTER, LABEL::REGULAR);

  show();
}

namespace {

void show() {
  ext_ = ExtUI::getMaterialPresetHotendTemp_celsius(index_);
  bed_ = ExtUI::getMaterialPresetBedTemp_celsius(index_);
  fan_ = ExtUI::getMaterialPresetFanSpeed_percent(index_);

  show_index();
  show_ext();
  show_bed();
  show_fan();
}

void show_index() {
  if(!index_text_) return;
  char text[8];
  sprintf(text, "%d/5", index_ + 1);
  lv_label_set_text(index_text_, text);
}

void show_ext() {
  if(!ext_text_) return;
  char text[12];
  sprintf(text, "%d°C", ext_);
  lv_label_set_text(ext_text_, text);
}

void show_bed() {
  if(!bed_text_) return;
  char text[12];
  sprintf(text, "%d°C", bed_);
  lv_label_set_text(bed_text_, text);
}

void show_fan() {
  if(!fan_text_) return;
  char text[12];
  sprintf(text, "%d%%", fan_);
  lv_label_set_text(fan_text_, text);
}

void on_back(lv_event_t *e) {
  PanelBase::switch_panel<ControlsPanel>();
}

void on_save(lv_event_t *e) {
  if(ext_ < HEATER_0_MINTEMP) ext_ = HEATER_0_MINTEMP;
  if(bed_ < BED_MINTEMP) bed_ = BED_MINTEMP;

  ExtUI::setMaterialPresetHotendTemp_celsius(index_, ext_);
  ExtUI::setMaterialPresetBedTemp_celsius(index_, bed_);
  ExtUI::setMaterialPresetFanSpeed_percent(index_, fan_);

  ExtUI::setTargetTemp_celsius(ext_, ExtUI::H0, true);
  ExtUI::setTargetTemp_celsius(bed_, ExtUI::BED, true);
  ExtUI::setTargetFan_percent(fan_, ExtUI::FAN0);
  ExtUI::saveSettings();

  Status::set(GET_TEXT(ADVD12_MSG_PREHEAT), Status::STATUS_OPTIONS::RESET);
  PanelBase::switch_panel<ControlsPanel>();
}

void on_previous(lv_event_t *e) {
  uint8_t max_index = ExtUI::getNbMaterialPresets() - 1;
  if(index_ <= 0) index_ = max_index; else index_--;
  show();
}

void on_next(lv_event_t *e) {
  uint8_t max_index = ExtUI::getNbMaterialPresets() - 1;
  if(index_ >= max_index) index_ = 0; else index_++;
  show();
}

void on_ext_minus(lv_event_t *e) {
  if(ext_ <= 0) return;
  ext_ -= 1;
  show_ext();
}

void on_ext_plus(lv_event_t *e) {
  if(ext_ >= HEATER_0_MAXTEMP) return;
  ext_ += 1;
  show_ext();
}

void on_bed_minus(lv_event_t *e) {
  if(bed_ <= 0) return;
  bed_ -= 1;
  show_bed();
}

void on_bed_plus(lv_event_t *e) {
  if(bed_ >= BED_MAXTEMP) return;
  bed_ += 1;
  show_bed();
}

void on_fan_minus(lv_event_t *e) {
  if(fan_ <= 0) return;
  fan_ -= 1;
  show_fan();
}

void on_fan_plus(lv_event_t *e) {
  if(fan_ >= 100) return;
  fan_ += 1;
  show_fan();
}

void on_cooldown(lv_event_t *e) {
  ExtUI::coolDown();
}

}
