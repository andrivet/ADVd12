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

#include "../../../inc/MarlinConfig.h"
#include <lvgl.h>
#include "infos.h"
#include "versions.h"
#include "copyrights.h"
#include "../panels.h"
#include "../main.h"
#include "../../core/ids.h"
#include "../../core/status.h"

namespace {

Panel<5> panel;

void on_back(lv_event_t *e);
void on_stats(lv_event_t *e);
void on_versions(lv_event_t *e);
void on_copyrights(lv_event_t *e);
void on_io(lv_event_t *e);
}

bool InfosPanel::can_switch() {
  return true;
}

void InfosPanel::create() {
  panel.create();
  panel.layout_menu("Infos");

  auto back = panel.control(IMG_LEFT, 7, 2);

  auto stats = panel.medium_button(IMG_STATS, "Stats", 19, 76);
  auto versions = panel.medium_button(IMG_VERSIONS, "Versions", 101, 76);
  auto copyrights = panel.medium_button(IMG_COPYRIGHTS, "Copyright", 183, 76);
  auto io = panel.medium_button(IMG_IO, "I/O", 265, 76);

  panel.reset_bindings();
  panel.bind(back, &on_back);
  panel.bind(stats, &on_stats);
  panel.bind(versions, &on_versions);
  panel.bind(copyrights, &on_copyrights);
  panel.bind(io, &on_io);
}

namespace {

void on_back(lv_event_t *e) {
  switch_panel<MainPanel>();
}

void on_stats(lv_event_t *e) {
  Status::set(GET_TEXT(ADVD12_MSG_NOT_IMPLEMENTED));
}

void on_versions(lv_event_t *e) {
  switch_panel<VersionsPanel>();
}

void on_copyrights(lv_event_t *e) {
  switch_panel<CopyrightsPanel>();
}

void on_io(lv_event_t *e) {
  Status::set(GET_TEXT(ADVD12_MSG_NOT_IMPLEMENTED));
}

}
