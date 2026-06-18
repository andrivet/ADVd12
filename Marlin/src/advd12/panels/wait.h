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

#include "../core/task.h"
#include "../marlin/types.h"

namespace Wait {
void wait();
void wait(const char* message);
void wait(void (*cb)(CALLBACK_SOURCE));
void wait(const char* message, void (*cb)(CALLBACK_SOURCE));
void wait_back(const char *title, const char *message, void (*cb)(CALLBACK_SOURCE));
void wait_back(const char* title, const char* message);
void wait_back_continue(const char* title, const char* message, void (*cb)(CALLBACK_SOURCE));
void wait_user(const char* title, const char *message, bool awaiting);
void homing(const FlashChar* command, void (*cb)());
void homing(const FlashChar* command = nullptr);
void homing_start();
void homing_done();
void ensure_homed(void (*cb)());
void not_busy(void (*cb)(CALLBACK_SOURCE));
void close();
bool check_not_busy();
};
