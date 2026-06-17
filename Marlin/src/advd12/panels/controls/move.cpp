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
#include "../../libs/ADVstd/bitmasks.h"
#include "../../core/ids.h"
#include "../../core/styles.h"
#include "../../core/core.h"
#include "../../core/status.h"
#include "../panels.h"
#include "move.h"
#include "controls.h"
#include "src/advd12/panels/wait.h"

namespace {

Panel<14> panel;

enum class Direction: uint8_t {
  None,
  X_PLUS,
  Y_PLUS,
  Z_PLUS,
  E_PLUS,
  MINUS   = 0x80,
  X_MINUS = MINUS + X_PLUS,
  Y_MINUS = MINUS + Y_PLUS,
  Z_MINUS = MINUS + Z_PLUS,
  E_MINUS = MINUS + E_PLUS
};

Direction direction_ = Direction::None;

constexpr feedRate_t FEEDRATE_X = 20;
constexpr feedRate_t FEEDRATE_Y = 20;
constexpr feedRate_t FEEDRATE_Z = 10;
constexpr feedRate_t FEEDRATE_E = 2;
constexpr unsigned TASK_DELAY = 250; // Has to be more than DEFAULT_TASK_DELAY

lv_obj_t *home(lv_coord_t x, lv_coord_t y, const char *symbol, const char *text);
void move(Direction direction);

void on_back(lv_event_t *e);
void on_disable(lv_event_t *e);
void on_x_minus(lv_event_t *e);
void on_x_plus(lv_event_t *e);
void on_y_minus(lv_event_t *e);
void on_y_plus(lv_event_t *e);
void on_z_minus(lv_event_t *e);
void on_z_plus(lv_event_t *e);
void on_e_minus(lv_event_t *e);
void on_e_plus(lv_event_t *e);
void on_home_x(lv_event_t *e);
void on_home_y(lv_event_t *e);
void on_home_z(lv_event_t *e);
void on_home_a(lv_event_t *e);

}

ENABLE_BITMASK_OPERATOR(Direction);

bool MovePanel::can_switch() {
  return true;
}

void MovePanel::create() {
  if(!panel.create()) return;
  panel.layout_menu("Move");
  panel.variables_time();

  auto back = panel.control(IMG_BACK, 7, 2);
  auto disable = panel.control(IMG_DISABLE, 289, 2);

  auto x_minus = panel.small_control(IMG_PREVIOUS, 24, 104);
  auto x_plus = panel.small_control(IMG_NEXT, 116, 104);

  auto y_minus = panel.small_control(IMG_DOWN, 70, 142);
  auto y_plus = panel.small_control(IMG_UP, 70, 66);

  auto z_minus = panel.small_control(IMG_DOWN, 194, 142);
  auto z_plus = panel.small_control(IMG_UP, 194, 66);

  auto e_plus = panel.small_control(IMG_DOWN, 278, 142);
  auto e_minus = panel.small_control(IMG_UP, 278, 66);

  auto home_x = home(21, 202, "X", "Home X");
  auto home_y = home(105, 202, "Y", "Home Y");
  auto home_z = home(189, 202, "Z", "Home Z");
  auto home_a = home(273, 202, "*", "All");

  panel.text("XY", 78, 112, LABEL::REGULAR);
  panel.text("Z", 207, 112, LABEL::REGULAR);
  panel.text("E", 291, 112, LABEL::REGULAR);

  panel.reset_bindings();
  panel.bind(back, &on_back);
  panel.bind(disable, &on_disable);
  panel.bind(x_minus, &on_x_minus);
  panel.bind(x_plus, &on_x_plus);
  panel.bind(y_minus, &on_y_minus);
  panel.bind(y_plus, &on_y_plus);
  panel.bind(z_minus, &on_z_minus);
  panel.bind(z_plus, &on_z_plus);
  panel.bind(e_minus, &on_e_minus);
  panel.bind(e_plus, &on_e_plus);
  panel.bind(home_x, &on_home_x);
  panel.bind(home_y, &on_home_y);
  panel.bind(home_z, &on_home_z);
  panel.bind(home_a, &on_home_a);
}

namespace {

lv_obj_t * home(lv_coord_t x, lv_coord_t y, const char *symbol, const char *text) {
  auto button = panel.control(IMG_HOME, x, y, text);

  auto label = lv_label_create(button);
  lv_label_set_text(label, symbol);
  lv_obj_add_style(label, &style_button_label, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_align(label, LV_ALIGN_CENTER, -1, 2);

  return button;
}

void on_back(lv_event_t *e) {
  PanelBase::switch_panel<ControlsPanel>();
}

void on_disable(lv_event_t *e) {
  advd12::Core::disable_motors();
}

void on_x_minus(lv_event_t *e) {
  move(Direction::X_MINUS);
}

void on_x_plus(lv_event_t *e) {
  move(Direction::X_PLUS);
}

void on_y_minus(lv_event_t *e) {
  move(Direction::Y_MINUS);
}

void on_y_plus(lv_event_t *e) {
  move(Direction::Y_PLUS);
}

void on_z_minus(lv_event_t *e) {
  move(Direction::Z_MINUS);
}

void on_z_plus(lv_event_t *e) {
  move(Direction::Z_PLUS);
}

void on_e_minus(lv_event_t *e) {
  move(Direction::E_MINUS);
}

void on_e_plus(lv_event_t *e) {
  move(Direction::E_PLUS);
}

void on_home_x(lv_event_t *e) {
  Wait::homing(F("G28 X"));
}

void on_home_y(lv_event_t *e) {
  Wait::homing(F("G28 Y"));
}

void on_home_z(lv_event_t *e) {
  Wait::homing(F("G28 Z"));
}

void on_home_a(lv_event_t *e) {
  Wait::homing(F("G28"));
}

// Do not use a struct table because it will take RAM space. So use fonctions instead.
float get_target() {
  switch(direction_) {
    case Direction::X_PLUS: return X_MAX_POS;
    case Direction::Y_PLUS: return Y_MAX_POS;
    case Direction::Z_PLUS: return Z_MAX_POS;
    case Direction::E_PLUS: return EXTRUDE_MAXLENGTH * 0.9;
    case Direction::X_MINUS: return X_MIN_POS;
    case Direction::Y_MINUS: return Y_MIN_POS;
    case Direction::Z_MINUS: return Z_MIN_POS;
    case Direction::E_MINUS: return -EXTRUDE_MAXLENGTH * 0.9;
    default: return 0;
  }
}

feedRate_t get_feedrate() {
  switch(get_cleared_bits(direction_, Direction::MINUS)) {
    case Direction::X_PLUS: return FEEDRATE_X;
    case Direction::Y_PLUS: return FEEDRATE_Y;
    case Direction::Z_PLUS: return FEEDRATE_Z;
    case Direction::E_PLUS: return FEEDRATE_E;
    default: break;
  }
  return FEEDRATE_X;
}

float get_position() {
  switch(get_cleared_bits(direction_, Direction::MINUS)) {
    case Direction::X_PLUS: return ExtUI::getAxisPosition_mm(ExtUI::X);
    case Direction::Y_PLUS: return ExtUI::getAxisPosition_mm(ExtUI::Y);
    case Direction::Z_PLUS: return ExtUI::getAxisPosition_mm(ExtUI::Z);
    case Direction::E_PLUS: return ExtUI::getAxisPosition_mm(ExtUI::E0);
    default: break;
  }
  return 0;
}

void set_position() {
  float new_position = get_target();
  auto current_position = get_position();
  if(current_position == new_position) return;
  auto feedrate = get_feedrate();

  switch(get_cleared_bits(direction_,Direction::MINUS)) {
    case Direction::X_PLUS: ExtUI::setAxisPosition_mm(new_position, ExtUI::X, feedrate); break;
    case Direction::Y_PLUS: ExtUI::setAxisPosition_mm(new_position, ExtUI::Y, feedrate); break;
    case Direction::Z_PLUS: ExtUI::setAxisPosition_mm(new_position, ExtUI::Z, feedrate); break;
    case Direction::E_PLUS: ExtUI::setAxisPosition_mm(new_position, ExtUI::E0, feedrate); break;
    default: break;
  }
}

void stop_move() {
  ExtUI::stopMove(); // Warning: will call idle() and thus is reentrant
  direction_ = Direction::None;
}

void move(Direction direction) {
  // First move?
  if(direction_ != direction) {
    direction_ = direction;
    background_task.set([] () -> CALLBACK_RESULT {
      stop_move();
      return CALLBACK_RESULT::STOP;
    }, TASK_DELAY);
    set_position();
  }
  else {
    background_task.rearm();
  }
}

}
