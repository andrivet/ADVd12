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
#include "progress.h"
#include "task.h"

namespace {

constexpr uint8_t WAIT_DELAY = 1; //!< Delay in minutes before displaying TC
enum struct STATE: uint8_t { IDLE, HEATING, WAITING, DISPLAYING }; //< Small state machine

uint8_t progress_at_wait_ = 0;
uint32_t elapsed_at_wait_ = 0;
unsigned previous_et_ = 0;
unsigned previous_progress_ = 0;
STATE state_ = STATE::IDLE;

void heating();
void waiting();
void displaying(lv_obj_t *et, lv_obj_t *tc, lv_obj_t *progress, bool force);

}

void Progress::set(lv_obj_t *et, lv_obj_t *tc, lv_obj_t *progress, bool force) {
  switch(state_) {
    case STATE::IDLE: break;
    case STATE::HEATING: heating(); break;
    case STATE::WAITING: waiting(); break;
    default: displaying(et, tc, progress, force); break;
  }
}

void Progress::reset() {
  start();
}

void Progress::start() {
  state_ = STATE::HEATING;
  previous_et_ = 0;
  previous_progress_ = 0;
}

void Progress::finish() {
  state_ = STATE::IDLE;
}

namespace {

void heating() {
  auto actual = ExtUI::getActualTemp_celsius(ExtUI::BED);
  auto target =  ExtUI::getTargetTemp_celsius(ExtUI::BED);
  if(target <= 0 || actual < static_cast<celsius_float_t>(target)) return;
  state_ = STATE::WAITING;
  elapsed_at_wait_ = ExtUI::getProgress_seconds_elapsed();
  progress_at_wait_ = ExtUI::getProgress_percent();
}

void waiting() {
  auto elapsed = ExtUI::getProgress_seconds_elapsed();
  if(elapsed - elapsed_at_wait_ < WAIT_DELAY * 60) return;
  state_ = STATE::DISPLAYING;
}

void displaying(lv_obj_t *et_label, lv_obj_t *tc_label, lv_obj_t *progress_label, bool force) {
  auto elapsed = ExtUI::getProgress_seconds_elapsed();
  auto progress = ExtUI::getProgress_percent();
  auto adjusted_progress = progress <= progress_at_wait_ ? 0 : progress - progress_at_wait_;
  auto adjusted_elapsed = elapsed <= elapsed_at_wait_ ? 0 : elapsed - elapsed_at_wait_;
  auto tc_sec = adjusted_progress <= 0 ? 0 : (adjusted_elapsed * (100 - adjusted_progress) / adjusted_progress);
  auto et_min = elapsed / 60;

  char text[24];

  // Update only when the ET (in minutes) change
  if(tc_label && (force || et_min != previous_et_)) {
    unsigned hours   = tc_sec / 3600;
    unsigned minutes = (tc_sec % 3600) / 60;
    sprintf(text, "%02u:%02u", hours, minutes);
    lv_label_set_text(tc_label, text);
  }

  if(et_label && (force || et_min != previous_et_)) {
    unsigned hours   = elapsed / 3600;
    unsigned minutes = (elapsed % 3600) / 60;
    sprintf(text, "%02u:%02u", hours, minutes);
    lv_label_set_text(et_label, text);
  }

  previous_et_ = et_min;

  if(progress_label && (force || previous_progress_ != progress)) {
    sprintf(text, "%d", progress);
    lv_label_set_text(progress_label, text);
    previous_progress_ = progress;
  }
}


}
