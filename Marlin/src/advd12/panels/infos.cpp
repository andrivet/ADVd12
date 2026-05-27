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


#include "infos.h"
#include "main.h"
#include "../core/ids.h"

void InfosPanel::prepare() {
  layout_menu("Infos");
  variables_time();

  auto back = control(IMG_BACK, 7, 2);

  auto stats = medium_button(IMG_STATS, "Stats", 19, 76);
  auto versions = medium_button(IMG_VERSIONS, "Versions", 101, 76);
  auto copyrights = medium_button(IMG_COPYRIGHTS, "Copyright", 183, 76);
  auto io = medium_button(IMG_IO, "I/O", 265, 76);

  reset_bindings();
  bind(back, &InfosPanel::on_back);
  bind(stats, &InfosPanel::on_stats);
  bind(versions, &InfosPanel::on_versions);
  bind(copyrights, &InfosPanel::on_copyrights);
  bind(io, &InfosPanel::on_io);
}

void InfosPanel::on_back(lv_event_t *e) {
  switch_panel(main_panel);
}

void InfosPanel::on_stats(lv_event_t *e) {

}

void InfosPanel::on_versions(lv_event_t *e) {

}

void InfosPanel::on_copyrights(lv_event_t *e) {

}

void InfosPanel::on_io(lv_event_t *e) {

}
