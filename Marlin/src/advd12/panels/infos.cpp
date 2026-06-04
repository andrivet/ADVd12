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
#include "panels.h"
#include "infos.h"
#include "main.h"
#include "../core/ids.h"

namespace {

Panel<5> infos_panel;

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
  if(!infos_panel.create()) return;
  infos_panel.layout_menu("Infos");
  infos_panel.variables_time();

  auto back = infos_panel.control(IMG_BACK, 7, 2);

  auto stats = infos_panel.medium_button(IMG_STATS, "Stats", 19, 76);
  auto versions = infos_panel.medium_button(IMG_VERSIONS, "Versions", 101, 76);
  auto copyrights = infos_panel.medium_button(IMG_COPYRIGHTS, "Copyright", 183, 76);
  auto io = infos_panel.medium_button(IMG_IO, "I/O", 265, 76);

  infos_panel.reset_bindings();
  infos_panel.bind(back, &on_back);
  infos_panel.bind(stats, &on_stats);
  infos_panel.bind(versions, &on_versions);
  infos_panel.bind(copyrights, &on_copyrights);
  infos_panel.bind(io, &on_io);
}

namespace {

void on_back(lv_event_t *e) {
  PanelBase::switch_panel<MainPanel>();
}

void on_stats(lv_event_t *e) {

}

void on_versions(lv_event_t *e) {

}

void on_copyrights(lv_event_t *e) {

}

void on_io(lv_event_t *e) {

}

}
