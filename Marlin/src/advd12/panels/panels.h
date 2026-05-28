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
  TIME
};

enum struct ALIGN {
  LEFT,
  RIGHT,
  CENTER
};


struct Panel {
  virtual ~Panel() = default;

protected:
  void create();
  virtual bool can_switch();
  virtual void prepare() = 0;

  lv_obj_t* rectangle(lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height);
  lv_obj_t* big_button(const char *img, const char *text, lv_coord_t x, lv_coord_t y);
  lv_obj_t* medium_button(const char *img, const char *text, lv_coord_t x, lv_coord_t y);
  lv_obj_t* control(const char *img, lv_coord_t x, lv_coord_t y, const char *text = nullptr);
  lv_obj_t* small_control(const char *img, lv_coord_t x, lv_coord_t y);
  lv_obj_t* icon(const char *img, lv_coord_t x, lv_coord_t y);
  lv_obj_t* text(const char *text, lv_coord_t x, lv_coord_t y, LABEL label);
  lv_obj_t* text(const char *text, lv_coord_t x, lv_coord_t y, lv_coord_t width, ALIGN align, LABEL kind);

  void switch_panel(Panel& panel);

  void layout_main();
  void layout_menu(const char *title);
  void variables_time();
  void variables_xyz();

private:
  void release();
  lv_obj_t* button(const char *img, const char *text, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, lv_coord_t label_y_offset);

private:
  lv_obj_t *scr_ = nullptr;
};

template<typename Derived, unsigned N>
struct PanelBase: Panel {
  using Handler = void (Derived::*)(lv_event_t *);

  void bind(lv_obj_t *obj, Handler handler, EventType type = EventType::PRESSED) {
    events_.bind(obj, static_cast<Derived*>(this), handler, type);
  }

  void reset_bindings() {
    events_.reset();
  }

private:
  EventBinder<Derived, N> events_;
};
