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

#include <lvgl.h>
#include <array>

enum struct EventType { PRESSED = 0x01, LONG = 0x02, REPEAT = 0x04 };

inline bool operator&(EventType lhs, EventType rhs) {
  return (static_cast<unsigned>(lhs) & static_cast<unsigned>(rhs)) != 0;
}

inline EventType operator|(EventType lhs, EventType rhs) {
  return static_cast<EventType>(static_cast<unsigned>(lhs) | static_cast<unsigned>(rhs));
}

template<typename T, unsigned N>
struct EventBinder {
  using Handler = void (T::*)(lv_event_t *);

  void bind(lv_obj_t *obj, T *self, Handler handler, EventType type) {
    auto binding = &bindings_[binding_count_++];
    binding->self = self;
    binding->handler = handler;

    if(type & EventType::PRESSED)
      lv_obj_add_event_cb(obj, &EventBinder::handler, LV_EVENT_PRESSED, binding);
    if(type & EventType::LONG)
      lv_obj_add_event_cb(obj, &EventBinder::handler, LV_EVENT_LONG_PRESSED, binding);
    if(type & EventType::REPEAT)
      lv_obj_add_event_cb(obj, &EventBinder::handler, LV_EVENT_LONG_PRESSED_REPEAT, binding);
  }

  void reset() {
    binding_count_ = 0;
  }

private:
  struct Binding {
    T *self;
    Handler handler;
  };

  std::array<Binding, N> bindings_;
  unsigned binding_count_ = 0;

  static void handler(lv_event_t *event) {
    auto *binding = static_cast<Binding *>(lv_event_get_user_data(event));
    if(binding && binding->self && binding->handler)
      (binding->self->*(binding->handler))(event);
  }
};
