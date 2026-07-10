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

#include "../../../lcd/extui/ui_api.h"
#include <lvgl.h>
#include "settings.h"
#include "../main.h"
#include "../../core/ids.h"
#include "../../core/status.h"
#include "../panels.h"
#include "../wait.h"

namespace {
Panel<7> settings_panel;

void on_back(lv_event_t *e);
void on_reset(lv_event_t *e);
void on_motors(lv_event_t *e);
void on_pid(lv_event_t *e);
void on_sensor(lv_event_t *e);
void on_brightness(lv_event_t *e);
void on_runout(lv_event_t *e);
}

bool SettingsPanel::can_switch() {
  return true;
}

void SettingsPanel::create() {
  if(!settings_panel.create()) return;
  settings_panel.layout_menu("Settings");

  auto back = settings_panel.control(IMG_LEFT, 7, 2);
  auto reset = settings_panel.control(IMG_RESET2, 289, 2);

  auto motors = settings_panel.medium_button(IMG_MOTOR, "Motors", 19, 76);
  auto pid = settings_panel.medium_button(IMG_PID, "PID", 101, 76);
  auto sensor = settings_panel.medium_button(IMG_SENSOR, " Sensor", 183, 76);
  auto brightness = settings_panel.medium_button(IMG_BRIGHTNESS, "LCD", 265, 76);
  auto runout = settings_panel.medium_button(IMG_RUNOUT, "Runout", 19, 188);

  settings_panel.reset_bindings();
  settings_panel.bind(back, &on_back);
  settings_panel.bind(reset, &on_reset);
  settings_panel.bind(motors, &on_motors);
  settings_panel.bind(pid, &on_pid);
  settings_panel.bind(sensor, &on_sensor);
  settings_panel.bind(brightness, &on_brightness);
  settings_panel.bind(runout, &on_runout);
}

namespace {

void on_back(lv_event_t *e) {
  PanelBase::switch_panel<MainPanel>();
}

void on_reset(lv_event_t *e) {
  Wait::wait_back_continue("Reset settings?", "Are you sure you want to reset settings?",
    [](CALLBACK_SOURCE src) {
      if(src == CALLBACK_SOURCE::BACK) return;
      ExtUI::resetSettings();
      ExtUI::saveSettings();
      ExtUI::setAllAxisUnhomed();
      ExtUI::setAllAxisPositionUnknown();
      Status::set(GET_TEXT(ADVD12_MSG_SETTINGS_RESET), Status::STATUS_OPTIONS::RESET);
    });
}

void on_motors(lv_event_t *e) {

}

void on_pid(lv_event_t *e) {

}

void on_sensor(lv_event_t *e) {

}

void on_brightness(lv_event_t *e) {

}

void on_runout(lv_event_t *e) {

}

}
