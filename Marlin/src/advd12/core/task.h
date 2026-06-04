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
#include "../../core/millis_t.h"

constexpr unsigned DEFAULT_TASK_DELAY = 100; // ms

enum struct CALLBACK_RESULT: uint8_t { STOP, CONTINUE };

//! Measure elapsed time
struct Elapse {
  unsigned delay_ = DEFAULT_TASK_DELAY;
  millis_t next_execute_time_ = 0;

  explicit Elapse(unsigned delay);

  bool is_elapsed(bool force = false);
  bool is_pending(bool force = false);
  void reset(unsigned delay = 0);
};

//! Task
struct Task {
  explicit Task(unsigned delay = DEFAULT_TASK_DELAY);

  void set(CALLBACK_RESULT (*callback)(), unsigned delay = DEFAULT_TASK_DELAY);
  void rearm();
  void clear();
  bool execute(bool force_execute = false);

private:
  Elapse elapse_;
  CALLBACK_RESULT (*callback_)() {nullptr};
};

inline Task background_task{}; // Primary task
inline Task status_task{}; // To update status task
inline Task wait_task{}; // For all wait screens
