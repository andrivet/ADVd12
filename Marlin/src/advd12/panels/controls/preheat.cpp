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
#include <array>
#include <lvgl.h>
#include "preheat.h"
#include "panel.h"
#include "controls.h"
#include "../../core/ids.h"
#include "src/advd12/panels/panels.h"

namespace {
constexpr unsigned NB_PREHEAT = 5;

unsigned index_ = 0;
std::array<Preheat, NB_PREHEAT> preheats_{};
lv_obj_t *index_text_ = nullptr;
lv_obj_t *ext_text_ = nullptr;
lv_obj_t *bed_text_ = nullptr;
lv_obj_t *fan_text_ = nullptr;

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
  preheat_panel.variables_time();

  auto back = preheat_panel.control(IMG_BACK, 7, 2);
  auto save = preheat_panel.control(IMG_SAVE, 289, 2);

  auto previous = preheat_panel.small_control(IMG_PREVIOUS, 53, 71);
  auto next = preheat_panel.small_control(IMG_NEXT, 173, 71);
  auto ext_minus = preheat_panel.small_control(IMG_MINUS, 53, 123);
  auto ext_plus = preheat_panel.small_control(IMG_PLUS, 173, 123);
  auto bed_minus = preheat_panel.small_control(IMG_MINUS, 53, 175);
  auto bed_plus = preheat_panel.small_control(IMG_PLUS, 173, 175);
  auto fan_minus = preheat_panel.small_control(IMG_MINUS, 53, 227);
  auto fan_plus = preheat_panel.small_control(IMG_PLUS, 173, 227);
  auto cooldown = preheat_panel.control(IMG_COOLDOWN, 262, 123, "Cooldown");

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

  index_text_ = preheat_panel.text("", 121, 79, LABEL::REGULAR);
  ext_text_ = preheat_panel.text("", 108, 131, LABEL::REGULAR);
  bed_text_ = preheat_panel.text("", 108, 183, LABEL::REGULAR);
  fan_text_ = preheat_panel.text("", 108, 235, LABEL::REGULAR);
  show_index();
  show_ext();
  show_bed();
  show_fan();
}

namespace {

void show_index() {
  if(!index_text_) return;
  char text[8];
  sprintf(text, "%d/5", index_ + 1);
  lv_label_set_text(index_text_, text);
}

void show_ext() {
  if(!ext_text_) return;
  char text[8];
  sprintf(text, "%d°C", preheats_[index_].extruder);
  lv_label_set_text(ext_text_, text);
}

void show_bed() {
  if(!bed_text_) return;
  char text[8];
  sprintf(text, "%d°C", preheats_[index_].bed);
  lv_label_set_text(bed_text_, text);
}

void show_fan() {
  if(!fan_text_) return;
  char text[8];
  sprintf(text, "%d%%", preheats_[index_].fan);
  lv_label_set_text(fan_text_, text);
}

void on_back(lv_event_t *e) {
  PanelBase::switch_panel<ControlsPanel>();
}

void on_save(lv_event_t *e) {
  // TODO: Preheat
  PanelBase::switch_panel<ControlsPanel>();
}

void on_previous(lv_event_t *e) {
  if(index_ <= 0) index_ = NB_PREHEAT - 1; else index_--;
  show_index();
}

void on_next(lv_event_t *e) {
  if(index_ >= NB_PREHEAT - 1) index_ = 0; else index_++;
  show_index();
}

void on_ext_minus(lv_event_t *e) {

}

void on_ext_plus(lv_event_t *e) {

}

void on_bed_minus(lv_event_t *e) {

}

void on_bed_plus(lv_event_t *e) {

}

void on_fan_minus(lv_event_t *e) {

}

void on_fan_plus(lv_event_t *e) {

}

void on_cooldown(lv_event_t *e) {

}

}
