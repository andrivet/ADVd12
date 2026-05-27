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
#include "load_unload.h"
#include "controls.h"
#include "../../core/ids.h"

void LoadUnloadPanel::prepare() {
  layout_menu("Load & Unload");
  variables_time();

  auto back = control(IMG_BACK, 7, 2);

  auto minus = small_control(IMG_MINUS, 93, 93);
  auto plus = small_control(IMG_PLUS, 213, 93);
  auto unload = control(IMG_UNLOAD, 89, 181, "Unload");
  auto load = control(IMG_LOAD2, 209, 181, "Load");

  reset_bindings();
  bind(back, &LoadUnloadPanel::on_back);
  bind(minus, &LoadUnloadPanel::on_minus, EventType::PRESSED | EventType::REPEAT);
  bind(plus, &LoadUnloadPanel::on_plus, EventType::PRESSED | EventType::REPEAT);
  bind(unload, &LoadUnloadPanel::on_unload);
  bind(load, &LoadUnloadPanel::on_load);

  temp_text_ = text("", 144, 101, LABEL::REGULAR);
  show_temp();
}

void LoadUnloadPanel::show_temp() {
  if(!temp_text_) return;
  char temp[8];
  sprintf(temp, "%d°C", temp_);
  lv_label_set_text(temp_text_, temp);
}

void LoadUnloadPanel::on_back(lv_event_t *e) {
  switch_panel(controls_panel);
}

void LoadUnloadPanel::on_minus(lv_event_t *e) {
  if(temp_ <= 0) return;
  temp_--;
  show_temp();
}

void LoadUnloadPanel::on_plus(lv_event_t *e) {
  if(temp_ >= 300) return;
  temp_++;
  show_temp();
}

void LoadUnloadPanel::on_unload(lv_event_t *e) {

}

void LoadUnloadPanel::on_load(lv_event_t *e) {

}
