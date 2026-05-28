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

struct PreheatPanel: PanelBase<PreheatPanel, 11> {
  struct Preheat {
    unsigned extruder;
    unsigned bed;
    unsigned fan;
  };

  static constexpr unsigned NB_PREHEAT = 5;

private:
  void prepare() override;
  void on_back(lv_event_t *e);
  void on_save(lv_event_t *e);
  void on_previous(lv_event_t *e);
  void on_next(lv_event_t *e);
  void on_ext_minus(lv_event_t *e);
  void on_ext_plus(lv_event_t *e);
  void on_bed_minus(lv_event_t *e);
  void on_bed_plus(lv_event_t *e);
  void on_fan_minus(lv_event_t *e);
  void on_fan_plus(lv_event_t *e);
  void on_cooldown(lv_event_t *e);
  void show_index() const;
  void show_ext() const;
  void show_bed() const;
  void show_fan() const;

private:
  unsigned index_ = 0;
  std::array<Preheat, NB_PREHEAT> preheats_{};
  lv_obj_t *index_text_ = nullptr;
  lv_obj_t *ext_text_ = nullptr;
  lv_obj_t *bed_text_ = nullptr;
  lv_obj_t *fan_text_ = nullptr;
};

inline PreheatPanel preheat_panel;
