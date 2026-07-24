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
#include "../calibration.h"

namespace {
Panel<7> panel;

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
  if(!panel.create()) return;
  panel.layout_menu("Settings");

  auto back = panel.control(IMG_LEFT, 7, 2);
  auto reset = panel.control(IMG_RESET2, 289, 2);

  auto motors = panel.medium_button(IMG_MOTOR, "Motors", 19, 76);
  auto pid = panel.medium_button(IMG_PID, "PID", 101, 76);
  auto sensor = panel.medium_button(IMG_SENSOR, " Sensor", 183, 76);
  auto brightness = panel.medium_button(IMG_BRIGHTNESS, "LCD", 265, 76);
  auto runout = panel.medium_button(IMG_RUNOUT, "Runout", 19, 188);

  panel.reset_bindings();
  panel.bind(back, &on_back);
  panel.bind(reset, &on_reset);
  panel.bind(motors, &on_motors);
  panel.bind(pid, &on_pid);
  panel.bind(sensor, &on_sensor);
  panel.bind(brightness, &on_brightness);
  panel.bind(runout, &on_runout);
}

namespace {

void on_back(lv_event_t *e) {
  switch_panel<MainPanel>();
}

void on_reset(lv_event_t *e) {
  Wait::wait_cancel_continue("Reset settings?", "Are you sure you want to reset settings?",
    [](CALLBACK_SOURCE src) {
      if(src == CALLBACK_SOURCE::BACK) return;
      ExtUI::resetSettings();
      ExtUI::saveSettings();
      ExtUI::setAllAxisUnhomed();
      ExtUI::setAllAxisPositionUnknown();
      Status::set(GET_TEXT(ADVD12_MSG_SETTINGS_RESET), Status::STATUS_OPTIONS::RESET);
      PanelBase::layout_reset();
      switch_panel<CalibrationPanel>();
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
