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
#include "controls.h"
#include "load_unload.h"
#include "move.h"
#include "preheat.h"
#include "../main.h"
#include "../panels.h"
#include "../../core/ids.h"
#include "../../../lcd/extui/ui_api.h"
#include "src/advd12/core/core.h"

namespace {
Panel<6> controls_panel;

void on_back(lv_event_t *e);
void on_load_unload(lv_event_t *e);
void on_preheat(lv_event_t *e);
void on_cooldown(lv_event_t *e);
void on_move(lv_event_t *e);
void on_disable_motors(lv_event_t *e);
}

bool ControlsPanel::can_switch() {
  return true;
}

void ControlsPanel::create() {
  if(!controls_panel.create()) return;
  controls_panel.layout_menu("Controls");
  controls_panel.variables_time();

  auto back = controls_panel.control(IMG_BACK, 7, 2);

  auto load   = controls_panel.medium_button(IMG_LOAD, "Load", 19, 76);
  auto heat   = controls_panel.medium_button(IMG_HEAT, "Preheat", 101, 76);
  auto cool   = controls_panel.medium_button(IMG_COOL, "Cooldown", 183, 76);
  auto move   = controls_panel.medium_button(IMG_MOVE, "Move", 265, 76);
  auto disable= controls_panel.medium_button(IMG_MOTOR, "Disable", 19, 188);

  controls_panel.reset_bindings();
  controls_panel.bind(back, &on_back);
  controls_panel.bind(load, &on_load_unload);
  controls_panel.bind(heat, &on_preheat);
  controls_panel.bind(cool, &on_cooldown);
  controls_panel.bind(move, &on_move);
  controls_panel.bind(disable, &on_disable_motors);
}

namespace {

void on_back(lv_event_t *e) {
  PanelBase::switch_panel<MainPanel>();
}

void on_load_unload(lv_event_t *e) {
  PanelBase::switch_panel<LoadUnloadPanel>();
}

void on_preheat(lv_event_t *e) {
  PanelBase::switch_panel<PreheatPanel>();
}

void on_cooldown(lv_event_t *e) {
  ExtUI::coolDown();
}

void on_move(lv_event_t *e) {
  PanelBase::switch_panel<MovePanel>();
}

void on_disable_motors(lv_event_t *e) {
  advd12::Core::disable_motors();
}

}
