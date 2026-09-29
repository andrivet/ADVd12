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
#include "versions.h"
#include "infos.h"
#include "../panels.h"
#include "../../core/core.h"
#include "../../core/ids.h"

using namespace advd12;

namespace {

Panel<1> panel;

void on_back(lv_event_t *e);
}

bool VersionsPanel::can_switch() {
  return true;
}

void VersionsPanel::create() {
  if(!panel.create()) return;
  panel.layout_menu("Versions");

  auto back = panel.control(IMG_LEFT, 7, 2);

  panel.label("ADVd12 version:", 40, 75, LABEL::GRAY);
  panel.label("ADVd12 build:", 40, 105, LABEL::GRAY);
  panel.label("Marlin version:", 40, 135, LABEL::GRAY);
  panel.label("Printer model:", 40, 165, LABEL::GRAY);
  panel.label("Mainboard:", 40, 195, LABEL::GRAY);
  panel.label("3DTouch:", 40, 225, LABEL::GRAY);

  panel.label(Core::version(), 180, 75, LABEL::REGULAR);
  panel.label(Core::build(), 180, 105, LABEL::REGULAR);
  panel.label(SHORT_BUILD_VERSION, 180, 135, LABEL::REGULAR);
  panel.label("D12-230", 180, 165, LABEL::REGULAR);
  panel.label(MOTHERBOARD_NAME, 180, 195, LABEL::REGULAR);

#if defined(BLTOUCH)
  panel.label("Yes", 180, 225, LABEL::REGULAR);
#else
  panel.label("No", 180, 225, LABEL::REGULAR);
#endif

  panel.reset_bindings();
  panel.bind(back, &on_back);
}

namespace {

void on_back(lv_event_t *e) {
  switch_panel<InfosPanel>();
}

}
