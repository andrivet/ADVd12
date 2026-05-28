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
#include "preheat.h"
#include "controls.h"
#include "../../core/ids.h"

void PreheatPanel::prepare() {
  layout_menu("Preheat");
  variables_time();

  auto back = control(IMG_BACK, 7, 2);
  auto save = control(IMG_SAVE, 289, 2);

  auto previous = small_control(IMG_PREVIOUS, 53, 71);
  auto next = small_control(IMG_NEXT, 173, 71);
  auto ext_minus = small_control(IMG_MINUS, 53, 123);
  auto ext_plus = small_control(IMG_PLUS, 173, 123);
  auto bed_minus = small_control(IMG_MINUS, 53, 175);
  auto bed_plus = small_control(IMG_PLUS, 173, 175);
  auto fan_minus = small_control(IMG_MINUS, 53, 227);
  auto fan_plus = small_control(IMG_PLUS, 173, 227);
  auto cooldown = control(IMG_COOLDOWN, 262, 123, "Cooldown");

  reset_bindings();
  bind(back, &PreheatPanel::on_back);
  bind(save, &PreheatPanel::on_save);
  bind(previous, &PreheatPanel::on_previous);
  bind(next, &PreheatPanel::on_next);
  bind(ext_minus, &PreheatPanel::on_ext_minus, EventType::PRESSED | EventType::REPEAT);
  bind(ext_plus, &PreheatPanel::on_ext_plus, EventType::PRESSED | EventType::REPEAT);
  bind(bed_minus, &PreheatPanel::on_bed_minus, EventType::PRESSED | EventType::REPEAT);
  bind(bed_plus, &PreheatPanel::on_bed_plus, EventType::PRESSED | EventType::REPEAT);
  bind(fan_minus, &PreheatPanel::on_fan_minus, EventType::PRESSED | EventType::REPEAT);
  bind(fan_plus, &PreheatPanel::on_fan_plus, EventType::PRESSED | EventType::REPEAT);
  bind(cooldown, &PreheatPanel::on_cooldown);

  index_text_ = text("", 121, 79, LABEL::REGULAR);
  ext_text_ = text("", 108, 131, LABEL::REGULAR);
  bed_text_ = text("", 108, 183, LABEL::REGULAR);
  fan_text_ = text("", 108, 235, LABEL::REGULAR);
  show_index();
  show_ext();
  show_bed();
  show_fan();
}

void PreheatPanel::show_index() const {
  if(!index_text_) return;
  char text[8];
  sprintf(text, "%d/5", index_ + 1);
  lv_label_set_text(index_text_, text);
}

void PreheatPanel::show_ext() const {
  if(!ext_text_) return;
  char text[8];
  sprintf(text, "%d°C", preheats_[index_].extruder);
  lv_label_set_text(ext_text_, text);
}

void PreheatPanel::show_bed() const {
  if(!bed_text_) return;
  char text[8];
  sprintf(text, "%d°C", preheats_[index_].bed);
  lv_label_set_text(bed_text_, text);
}

void PreheatPanel::show_fan() const {
  if(!fan_text_) return;
  char text[8];
  sprintf(text, "%d%%", preheats_[index_].fan);
  lv_label_set_text(fan_text_, text);
}

void PreheatPanel::on_back(lv_event_t *e) {
  switch_panel(controls_panel);
}

void PreheatPanel::on_save(lv_event_t *e) {
  // TODO: Preheat
  switch_panel(controls_panel);
}

void PreheatPanel::on_previous(lv_event_t *e) {
  if(index_ <= 0) index_ = NB_PREHEAT - 1; else index_--;
  show_index();
}

void PreheatPanel::on_next(lv_event_t *e) {
  if(index_ >= NB_PREHEAT - 1) index_ = 0; else index_++;
  show_index();
}

void PreheatPanel::on_ext_minus(lv_event_t *e) {

}

void PreheatPanel::on_ext_plus(lv_event_t *e) {

}

void PreheatPanel::on_bed_minus(lv_event_t *e) {

}

void PreheatPanel::on_bed_plus(lv_event_t *e) {

}

void PreheatPanel::on_fan_minus(lv_event_t *e) {

}

void PreheatPanel::on_fan_plus(lv_event_t *e) {

}

void PreheatPanel::on_cooldown(lv_event_t *e) {

}
