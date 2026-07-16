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
#include "main.h"
#include "panels.h"
#include "controls/controls.h"
#include "leveling/leveling.h"
#include "print/sd.h"
#include "print/print.h"
#include "tuning/tuning.h"
#include "settings/settings.h"
#include "infos/infos.h"
#include "../core/ids.h"
#include "src/lcd/extui/ui_api.h"

namespace {
Panel<6> panel;

void on_controls(lv_event_t *e);
void on_leveling(lv_event_t *e);
void on_print(lv_event_t *e);
void on_tuning(lv_event_t *e);
void on_settings(lv_event_t *e);
void on_info(lv_event_t *e);
}

bool MainPanel::can_switch() {
  return true;
}

void MainPanel::create() {
  if(!panel.create()) return;
  panel.layout_main();

  auto controls = panel.big_button(IMG_CONTROLS, "Controls", 36, 34);
  auto leveling = panel.big_button(IMG_LEVELING, "Leveling", 140, 34);
  auto print    = panel.big_button(IMG_PRINT, "Printing", 244, 34);
  auto tuning   = panel.big_button(IMG_TUNING, "Tuning", 36, 154);
  auto settings = panel.big_button(IMG_SETTINGS, "Settings", 140, 154);
  auto info     = panel.big_button(IMG_INFO, "Info", 244, 154);

  panel.reset_bindings();
  panel.bind(controls, &on_controls);
  panel.bind(leveling, &on_leveling);
  panel.bind(print, &on_print);
  panel.bind(tuning, &on_tuning);
  panel.bind(settings, &on_settings);
  panel.bind(info, &on_info);
}

namespace {

void on_controls(lv_event_t *e) {
  switch_panel<ControlsPanel>();
}

void on_leveling(lv_event_t *e) {
  switch_panel<LevelingPanel>();
}

void on_print(lv_event_t *e) {
  switch_panel<SdPanel>();
}

void on_tuning(lv_event_t *e) {
  switch_panel<TuningPanel>();
}

void on_settings(lv_event_t *e) {
  switch_panel<SettingsPanel>();
}

void on_info(lv_event_t *e) {
  switch_panel<InfosPanel>();
}

}
