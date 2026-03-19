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

#include "main.h"
#include "../core/ids.h"

MainPanel main_panel;

void MainPanel::draw() {
  panel();

  button(IMG_CONTROLS, "Controls", 28, 34, 64, 64, [this] { return on_controls(); });
  button(IMG_LEVELING, "Leveling", 124, 34, 64, 64, [this] { return on_leveling(); });
  button(IMG_PRINT, "Printing", 220, 34, 64, 64, [this] { return on_print(); });
  button(IMG_TUNING, "Tools", 28, 154, 64, 64, [this] { return on_tuning(); });
  button(IMG_SETTINGS, "Settings", 124, 154, 64, 64, [this] { return on_settings(); });
  button(IMG_INFO, "Info", 220, 154, 64, 64, [this] { return on_info(); });

  rectangle(330, 0, 3, 284);
  rectangle(0, 284, 480, 3);

  icon(IMG_EXTRUDER, 363, 20);
  text("198.0", 398, 9, 64, ALIGN::RIGHT, LABEL::EXTRUDER);
  text("200", 398, 37, 64, ALIGN::RIGHT, LABEL::EXTRUDER);

  icon(IMG_BED, 363, 87);
  text("100.6", 398, 73, 64, ALIGN::RIGHT, LABEL::BED);
  text("100", 398, 101, 64, ALIGN::RIGHT, LABEL::BED);

  icon(IMG_FAN, 363, 149);
  text("% 100", 398, 149 + 4, 64, ALIGN::RIGHT, LABEL::FAN);

  icon(IMG_ET, 363, 197);
  text("00:32", 398, 197 + 4, 64, ALIGN::RIGHT, LABEL::TIME);
  icon(IMG_TC, 363, 245);
  text("12:34", 398, 245 + 4, 64, ALIGN::RIGHT, LABEL::TIME);

}

void MainPanel::on_controls() {

}

void MainPanel::on_leveling() {

}

void MainPanel::on_print() {

}

void MainPanel::on_tuning() {

}

void MainPanel::on_settings() {

}

void MainPanel::on_info() {

}
