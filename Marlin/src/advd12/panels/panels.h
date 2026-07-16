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
#include <functional>
#include <lvgl.h>
#include "events.h"

enum struct LABEL {
  REGULAR,
  EXTRUDER,
  BED,
  FAN,
  X = EXTRUDER,
  Y = BED,
  Z = FAN,
  GRAY
};

enum struct ALIGN {
  LEFT,
  RIGHT,
  CENTER
};

template<typename T>
static void switch_panel();

struct PanelBase {
  bool create();
  void release();
  [[nodiscard]] lv_obj_t* scr() const { return scr_; }

  lv_obj_t* rectangle(lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height);
  lv_obj_t* big_button(const char *img, const char *text, lv_coord_t x, lv_coord_t y);
  lv_obj_t* medium_button(const char *img, const char *text, lv_coord_t x, lv_coord_t y);
  lv_obj_t* control(const char *img, lv_coord_t x, lv_coord_t y, const char *text = nullptr);
  lv_obj_t* circle_button(lv_coord_t x, lv_coord_t y, const char *text);
  lv_obj_t* icon(const char *img, lv_coord_t x, lv_coord_t y);
  lv_obj_t* text(const char *text, lv_coord_t x, lv_coord_t y, LABEL label);
  lv_obj_t* text(const char *text, lv_coord_t x, lv_coord_t y, lv_coord_t width, ALIGN align, LABEL kind);
  lv_obj_t* text(double value, lv_coord_t x, lv_coord_t y, lv_coord_t width, ALIGN align, LABEL kind);
  lv_obj_t* text(int value, lv_coord_t x, lv_coord_t y, lv_coord_t width, ALIGN align, LABEL kind);
  lv_obj_t* label(const char *text, lv_coord_t x, lv_coord_t y, LABEL label);
  lv_obj_t* label(const char *text, lv_coord_t x, lv_coord_t y, lv_coord_t width, ALIGN align, LABEL kind);
  lv_obj_t* label(double value, lv_coord_t x, lv_coord_t y, lv_coord_t width, ALIGN align, LABEL kind);
  lv_obj_t* label(int value, lv_coord_t x, lv_coord_t y, lv_coord_t width, ALIGN align, LABEL kind);

  void layout_main();
  void layout_menu(const char *title);
  void variables();
  static void update_variables(bool force = false);
  static void set_status(const char *status);

private:
  lv_obj_t* button(const char *img, const char *text, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, lv_coord_t label_y_offset);

private:
  lv_obj_t *scr_ = nullptr;
  static PanelBase *current_panel_;
};

template<typename T>
void switch_panel() {
  if(!T::can_switch()) return;
  T::create();
}

template<unsigned N>
struct Panel: PanelBase {
  using Handler = void(*)(lv_event_t *);

  void bind(lv_obj_t *obj, Handler handler, EventType type = EventType::PRESSED) {
    events_.bind(obj, handler, type);
  }

  void reset_bindings() {
    events_.reset();
  }

private:
  EventBinder<N> events_{};
};
