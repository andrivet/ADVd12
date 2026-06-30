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

#include "../../inc/MarlinConfig.h"
#include "../../lcd/extui/ui_api.h"
#include "status.h"
#include "task.h"
#include "../panels/panels.h"

namespace {

millis_t status_message_expire_ms = 0;

}

namespace Status {

void set(const char* message, STATUS_OPTIONS options) {
  if(test_one_bit(options, STATUS_OPTIONS::RESET)) reset();
  bool persistent = test_one_bit(options, STATUS_OPTIONS::PERSISTENT);
  // If a persistent message is already displayed, do not override it my a non-persistent one
  if(status_message_expire_ms == 0 && !persistent) return;
  status_message_expire_ms = persistent ? 0 : millis() + (STATUS_MESSAGE_TIMEOUT_SEC) * 1000UL;
  PanelBase::set_status(message);
}

void set_default() {
  set(GET_TEXT(WELCOME_MSG), Status::STATUS_OPTIONS::RESET);
}

void reset() {
  status_message_expire_ms = millis() + (STATUS_MESSAGE_TIMEOUT_SEC) * 1000UL;
}

void init() {
  status_task.set([] () -> CALLBACK_RESULT {
      if(status_message_expire_ms && ELAPSED(millis(), status_message_expire_ms))
        ExtUI::resetStatus();
      return CALLBACK_RESULT::CONTINUE;
    });
}

}
