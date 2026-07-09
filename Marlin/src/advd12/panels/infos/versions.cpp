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
#include "../../../inc/Version.h"
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

  panel.text("ADVd12 version:", 7, 76, LABEL::GRAY);
  panel.text("ADVd12 build:", 7, 106, LABEL::GRAY);
  panel.text("Marlin version:", 7, 136, LABEL::GRAY);
  panel.text("Printer model:", 7, 166, LABEL::GRAY);
  panel.text("Mainboard:", 7, 196, LABEL::GRAY);
  panel.text("BLTouch:", 7, 226, LABEL::GRAY);

  panel.text(Core::version(), 174, 76, LABEL::REGULAR);
  panel.text(Core::build(), 174, 106, LABEL::REGULAR);
  panel.text(SHORT_BUILD_VERSION, 174, 136, LABEL::REGULAR);
  panel.text("D12-230", 174, 166, LABEL::REGULAR);

#if defined(D12_230_v1_2)
  panel.text("Robin Nano V1.2", 174, 196, LABEL::REGULAR);
#elif defined(D12_230_v1_3)
  panel.text("Robin Nano V1.3", 174, 196, LABEL::REGULAR);
#elif defined(D12_SIMULATOR)
  panel.text("Simulator", 174, 196, LABEL::REGULAR);
#else
  panel.text("Unknown", 174, 196, LABEL::REGULAR);
#endif

#if defined(BLTOUCH)
  panel.text("Yes", 174, 226, LABEL::REGULAR);
#else
  panel.text("No", 174, 226, LABEL::REGULAR);
#endif

  panel.reset_bindings();
  panel.bind(back, &on_back);
}

namespace {

void on_back(lv_event_t *e) {
  PanelBase::switch_panel<InfosPanel>();
}

}
