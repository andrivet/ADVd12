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

#pragma once
#include "../panels.h"

struct TuningPanel: PanelBase<TuningPanel, 8> {
private:
  void prepare() override;
  void on_back(lv_event_t *e);
  void on_bltouch(lv_event_t *e);
  void on_extruder(lv_event_t *e);
  void on_pid(lv_event_t *e);
  void on_vibrations(lv_event_t *e);
  void on_shaping(lv_event_t *e);
  void on_skew(lv_event_t *e);
  void on_linear_advance(lv_event_t *e);
};

inline TuningPanel tuning_panel;
