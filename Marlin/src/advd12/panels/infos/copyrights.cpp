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
#include "copyrights.h"
#include "infos.h"
#include "../panels.h"
#include "../../core/core.h"
#include "../../core/ids.h"

using namespace advd12;

namespace {

Panel<1> panel;

void on_back(lv_event_t *e);
}

bool CopyrightsPanel::can_switch() {
  return true;
}

void CopyrightsPanel::create() {
  if(!panel.create()) return;
  panel.layout_menu("Copyrights");

  auto back = panel.control(IMG_LEFT, 7, 2);

  panel.text("ADVd12", 9, 64, 331, ALIGN::CENTER, LABEL::REGULAR);
  panel.text("Copyright (c) 2026\nSebastien Andrivet", 9, 91, 331, ALIGN::CENTER, LABEL::GRAY);

  panel.text("Marlin", 9, 147, 331, ALIGN::CENTER, LABEL::REGULAR);
  panel.text("Copyright (c) 2026\nMarlinFirmware", 9, 174, 331, ALIGN::CENTER, LABEL::GRAY);
  panel.text("Copyright (c) 2011 Camiel\nGubbels, Eric van der Zalm", 9, 223, 331, ALIGN::CENTER, LABEL::GRAY);

  panel.reset_bindings();
  panel.bind(back, &on_back);
}

namespace {

void on_back(lv_event_t *e) {
  PanelBase::switch_panel<InfosPanel>();
}

}
