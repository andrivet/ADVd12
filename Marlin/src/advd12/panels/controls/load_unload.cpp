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
#include "load_unload.h"
#include "controls.h"
#include "../panels.h"
#include "../wait.h"
#include "../../core/ids.h"
#include "../../core/core.h"
#include "../../core/task.h"
#include "../../core/status.h"
#include "src/advd12/core/logging.h"

namespace {

float previous_z_ = 0;
celsius_t temp_ = 200;
bool load_ = true;
float extrude_length_ = 0.;
lv_obj_t *temp_text_ = nullptr;

Panel<7> panel;

void on_back(lv_event_t *e);
void on_minus(lv_event_t *e);
void on_plus(lv_event_t *e);
void on_partial_unload(lv_event_t *e);
void on_full_unload(lv_event_t *e);
void on_partial_load(lv_event_t *e);
void on_full_load(lv_event_t *e);
void show_temp();
void prepare(unsigned length, bool load);
void extrude();
}

bool LoadUnloadPanel::can_switch() {
  return Wait::check_not_busy();
}

void LoadUnloadPanel::create() {
  if(!panel.create()) return;
  panel.layout_menu("Load & Unload");

  auto back = panel.control(IMG_LEFT, 7, 2);

  auto minus = panel.control(IMG_MINUS, 93, 80);
  auto plus = panel.control(IMG_PLUS, 213, 80);
  auto partial_unload = panel.control(IMG_UNLOAD, 24, 189, "Partial");
  auto full_unload = panel.control(IMG_UNLOAD, 108, 189, "Full");
  auto partial_load = panel.control(IMG_LOAD2, 192, 189, "Partial");
  auto full_load = panel.control(IMG_LOAD2, 276, 189, "Full");

  temp_text_ = panel.text("", 141, 93, 72, ALIGN::CENTER, LABEL::REGULAR);
  panel.text("Unload", 8, 152, 164, ALIGN::CENTER, LABEL::REGULAR);
  panel.text("Load", 176, 152, 164, ALIGN::CENTER, LABEL::REGULAR);

  panel.reset_bindings();
  panel.bind(back, &on_back);
  panel.bind(minus, &on_minus, EventType::PRESSED | EventType::REPEAT);
  panel.bind(plus, &on_plus, EventType::PRESSED | EventType::REPEAT);
  panel.bind(partial_unload, &on_partial_unload);
  panel.bind(full_unload, &on_full_unload);
  panel.bind(partial_load, &on_partial_load);
  panel.bind(full_load, &on_full_load);

  show_temp();

  Status::reset();
  previous_z_ = advd12::Core::ensure_z_enough_room();
  temp_ = ExtUI::getDefaultTemp_celsius(ExtUI::H0);
}

namespace {

void show_temp() {
  if(!temp_text_) return;
  char temp[12];
  sprintf(temp, "%d°C", temp_);
  lv_label_set_text(temp_text_, temp);
}

void on_back(lv_event_t *e) {
  ExtUI::setTargetTemp_celsius(0, ExtUI::H0);
  ExtUI::setAxisPosition_mm(previous_z_, ExtUI::Z, 20);
  switch_panel<ControlsPanel>();
}

void on_minus(lv_event_t *e) {
  if(temp_ <= 0) return;
  temp_--;
  show_temp();
}

void on_plus(lv_event_t *e) {
  if(temp_ >= 300) return;
  temp_++;
  show_temp();
}

void on_partial_unload(lv_event_t *e) {
  prepare(65, false);
}

void on_full_unload(lv_event_t *e) {
  prepare(650, false);
}

void on_partial_load(lv_event_t *e) {
  prepare(65, true);
}

void on_full_load(lv_event_t *e) {
  prepare(650, true);
}

//! Prepare Load or Unload step #1: set the target temperature, setup the next step and display a wait message
void prepare(unsigned length, bool load) {
  load_ = load;
  extrude_length_ = static_cast<float>(length);

  ExtUI::setTargetTemp_celsius(temp_, ExtUI::H0, true);
  if(ExtUI::getDefaultTemp_celsius(ExtUI::H0) != temp_) {
    ExtUI::setDefaultTemp_celsius(temp_, ExtUI::H0);
    ExtUI::saveSettings();
  }

  ExtUI::setHostKeepaliveState(GcodeSuite::IN_PROCESS);
  Wait::heating([](CALLBACK_SOURCE src) -> void {
    if(src == CALLBACK_SOURCE::BACK) {
      Status::set(GET_TEXT(ADVD12_MSG_CANCELED), Status::STATUS_OPTIONS::RESET);
      ExtUI::setHostKeepaliveState(GcodeSuite::NOT_BUSY);
      ExtUI::setTargetTemp_celsius(0, ExtUI::H0);
      return;
    }

    extrude();
  });
}

void extrude() {
  auto current = ExtUI::getAxisPosition_mm(ExtUI::E0);
  auto length = load_ ? extrude_length_ : -extrude_length_;
  auto new_position = current + length;
  Log::verbose() << "E: current = " << current << ", length = " << length << ", new = " << new_position << Log::endl();
  ExtUI::setAxisPosition_mm(
    new_position,
    ExtUI::E0,
    load_ ? 6 : 10
  );
  Log::verbose() << "E: current = " << ExtUI::getAxisPosition_mm(ExtUI::E0) << Log::endl();

  auto title = load_ ? GET_TEXT(MSG_FILAMENT_CHANGE_LOAD) : GET_TEXT(MSG_FILAMENT_CHANGE_UNLOAD);
  Wait::wait_cancel(title, GET_TEXT(ADVD12_MSG_IN_PROGRESS), [] (CALLBACK_SOURCE src) -> void {
    if(src == CALLBACK_SOURCE::BACK) {
      Status::set(GET_TEXT(ADVD12_MSG_CANCELED), Status::STATUS_OPTIONS::RESET);
      ExtUI::setHostKeepaliveState(GcodeSuite::NOT_BUSY);
      ExtUI::stopMove();
    }
  });

  background_task.set([] () -> CALLBACK_RESULT {
    if(ExtUI::isMoving()) return CALLBACK_RESULT::CONTINUE;
    ExtUI::setHostKeepaliveState(GcodeSuite::NOT_BUSY);
    Status::set_default();
    Wait::close();
    return CALLBACK_RESULT::STOP;
  });
}

}
