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


#include "controls.h"
#include "load_unload.h"
#include "preheat.h"
#include "../main.h"
#include "../../core/ids.h"

void ControlsPanel::prepare() {
  layout_menu("Controls");
  variables_time();

  auto back = control(IMG_BACK, 7, 2);

  auto load   = medium_button(IMG_LOAD, "Load", 19, 76);
  auto heat   = medium_button(IMG_HEAT, "Preheat", 101, 76);
  auto cool   = medium_button(IMG_COOL, "Cooldown", 183, 76);
  auto move   = medium_button(IMG_MOVE, "Move", 265, 76);
  auto disable= medium_button(IMG_MOTOR, "Disable", 19, 188);

  reset_bindings();
  bind(back, &ControlsPanel::on_back);
  bind(load, &ControlsPanel::on_load_unload);
  bind(heat, &ControlsPanel::on_preheat);
  bind(cool, &ControlsPanel::on_cooldown);
  bind(move, &ControlsPanel::on_move);
  bind(disable, &ControlsPanel::on_disable_motors);
}

void ControlsPanel::on_back(lv_event_t *e) {
  switch_panel(main_panel);
}

void ControlsPanel::on_load_unload(lv_event_t *e) {
  switch_panel(load_unload_panel);
}

void ControlsPanel::on_preheat(lv_event_t *e) {
  switch_panel(preheat_panel);
}

void ControlsPanel::on_cooldown(lv_event_t *e) {

}

void ControlsPanel::on_move(lv_event_t *e) {

}

void ControlsPanel::on_disable_motors(lv_event_t *e) {

}
