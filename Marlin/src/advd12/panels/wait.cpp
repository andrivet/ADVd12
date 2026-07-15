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

#include <lvgl.h>
#include "../../inc/MarlinConfig.h"
#include "../../lcd/extui/ui_api.h"
#include "wait.h"
#include "events.h"
#include "graph.h"
#include "../core/core.h"
#include "../core/status.h"
#include "../core/styles.h"
#include "../core/ids.h"

namespace {
lv_obj_t *modal = nullptr;
lv_obj_t *msgbox = nullptr;
EventBinder<1> binder{};
void (*wait_cb)(CALLBACK_SOURCE) = nullptr;
void (*homing_cb)() = nullptr;
bool close_on_homing = false;

lv_obj_t *wait_base(const char *title, const char *message, void (*cb)(CALLBACK_SOURCE));
lv_obj_t *wait_button(const char *src, lv_coord_t x, lv_coord_t y);
void on_back(lv_event_t *event);
void on_continue(lv_event_t *event);
void on_confirm(lv_event_t *event);
void on_cancel_heating(lv_event_t *event);
}


void Wait::wait() {
  wait_base("Wait", "Please wait...", nullptr);
}

void Wait::wait(const char * message) {
  wait_base("Wait", message, nullptr);
}

void Wait::wait(void (*cb)(CALLBACK_SOURCE)) {
  wait_base("Wait", "Please wait...", cb);
}

void Wait::wait(const char *message, void (*cb)(CALLBACK_SOURCE)) {
  wait_base("Wait", message, cb);
}

void Wait::wait_back(const char *title, const char *message, void (*cb)(CALLBACK_SOURCE)) {
  wait_base(title, message, cb);

  auto btn = wait_button(IMG_LEFT, 4, 4);
  binder.reset();
  binder.bind(btn, &on_back);
}

void Wait::wait_back(const char * title, const char * message) {
  wait_base(title, message, nullptr);

  auto btn = wait_button(IMG_LEFT, 4, 4);
  binder.reset();
  binder.bind(btn, &on_back);
}

void Wait::wait_back_continue(const char * title, const char * message, void (*cb)(CALLBACK_SOURCE)) {
  wait_base(title, message, cb);

  auto back = wait_button(IMG_LEFT, 4, 4);
  auto cont = wait_button(IMG_SAVE, 276, 4);
  binder.reset();
  binder.bind(back, &on_back);
  binder.bind(cont, &on_continue);
}

void Wait::wait_user(const char * title, const char *message, bool awaiting) {
  wait_base(title, message, nullptr);

  if(awaiting) {
    auto confirm = wait_button(IMG_CONFIRM, 140, 173);
    binder.reset();
    binder.bind(confirm, &on_confirm);
  }
}

void Wait::homing(const FlashChar* command, void (*cb)()) {
  homing_cb = cb;
  close_on_homing = true;
  wait_base("Homing", "Please wait...", nullptr);
  ExtUI::injectCommands(command == nullptr ? F("G28") : command);
}

void Wait::homing(const FlashChar* command) {
  homing_cb = nullptr;
  close_on_homing = true;
  wait_base("Homing", "Please wait...", nullptr);
  ExtUI::injectCommands(command == nullptr ? F("G28") : command);
}

void Wait::homing_start() {
  Status::set(GET_TEXT(MSG_HOMING), Status::STATUS_OPTIONS::RESET);
}

void Wait::homing_done() {
  if(close_on_homing) {
    close_on_homing = false;
    close();
  }
  if(homing_cb) {
    homing_cb();
    homing_cb = nullptr;
  }
}

void Wait::ensure_homed(void (*cb)()) {
  if(
    ExtUI::isAxisPositionKnown(ExtUI::X) &&
    ExtUI::isAxisPositionKnown(ExtUI::Y) &&
    ExtUI::isAxisPositionKnown(ExtUI::Z)
  ) {
    cb();
    return;
  }

  homing(nullptr, cb);
}

void Wait::not_busy(void (*cb)(CALLBACK_SOURCE)) {
  wait_task.set([]() -> CALLBACK_RESULT {
    if(advd12::Core::is_busy()) return CALLBACK_RESULT::CONTINUE;
    close();
    if(wait_cb) wait_cb(CALLBACK_SOURCE::CONTINUE);
    return CALLBACK_RESULT::STOP;
  });
  wait(cb);
}

void Wait::close() {
  if(msgbox) {
    lv_obj_del(msgbox);
    msgbox = nullptr;
  }

  if(modal) {
    lv_obj_del(modal);
    modal = nullptr;
  }
}

bool Wait::check_not_busy() {
  if(!advd12::Core::is_busy()) return true;
  wait_back(GET_TEXT(ADVD12_TITLE_WARNING), GET_TEXT(ADVD12_MSG_BUSY));
  return false;
}

void Wait::heating(void (*cb)(CALLBACK_SOURCE)) {
  wait_task.set([]() -> CALLBACK_RESULT {
    if(
      ExtUI::getActualTemp_celsius(ExtUI::E0) < static_cast<celsius_float_t>(ExtUI::getTargetTemp_celsius(ExtUI::E0)) ||
      ExtUI::getActualTemp_celsius(ExtUI::BED) < static_cast<celsius_float_t>(ExtUI::getTargetTemp_celsius(ExtUI::BED))
    ) {
      Graph::next_point();
      return CALLBACK_RESULT::CONTINUE;
    }

    close();
    if(wait_cb) wait_cb(CALLBACK_SOURCE::CONTINUE);
    return CALLBACK_RESULT::STOP;
  }, 500);

  auto box = wait_base("Heating", nullptr, cb);
  Graph::create(box);

  auto cancel = wait_button(IMG_CANCEL, 4, 4);
  binder.reset();
  binder.bind(cancel, &on_cancel_heating);
}

namespace {

void on_cancel_heating(lv_event_t *event) {
  wait_task.clear();
  Wait::close();
  if(wait_cb) wait_cb(CALLBACK_SOURCE::BACK);
}

lv_obj_t *wait_base(const char *title, const char *message, void (*cb)(CALLBACK_SOURCE)) {
  if(modal || msgbox) Wait::close();

  wait_cb = cb;
  auto parent = lv_scr_act();

  modal = lv_obj_create(parent);
  lv_obj_set_size(modal, LV_PCT(100), LV_PCT(100));
  lv_obj_center(modal);
  lv_obj_add_flag(modal, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_style_bg_color(modal, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(modal, LV_OPA_20, LV_PART_MAIN | LV_STATE_DEFAULT);

  msgbox = lv_obj_create(parent);
  lv_obj_set_size(msgbox, 329, 233);
  lv_obj_set_pos(msgbox, 7, 40);
  lv_obj_set_style_bg_color(msgbox, lv_color_black(), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(msgbox, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(msgbox, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(msgbox, lv_color_make(255, 148, 0), LV_PART_MAIN | LV_STATE_DEFAULT);

  auto line = lv_obj_create(msgbox);
  lv_obj_set_pos(line, 0, 55);
  lv_obj_set_size(line, 329, 3);
  lv_obj_set_style_bg_color(line, orange, LV_PART_MAIN);
  lv_obj_set_style_bg_opa(line, LV_OPA_COVER, LV_PART_MAIN);

  auto label = lv_label_create(msgbox);
  lv_label_set_text(label, title);
  lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
  lv_obj_align_to(label, line, LV_ALIGN_BOTTOM_MID, 0, -18);

  if(message) {
    auto text = lv_label_create(msgbox);
    lv_obj_set_style_text_color(text, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_text_font(text, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_obj_set_style_text_align(text, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_width(text, lv_pct(100));
    lv_label_set_text(text, message);
    lv_obj_set_pos(text, 0, 126);
  }

  return msgbox;
}

lv_obj_t *wait_button(const char *src, lv_coord_t x, lv_coord_t y) {
  lv_obj_t * btn = lv_imgbtn_create(msgbox);
  lv_obj_set_pos(btn, x, y);
  lv_obj_set_width(btn, 48);
  lv_obj_set_height(btn, 48);

  lv_imgbtn_set_src(btn, LV_IMGBTN_STATE_RELEASED, nullptr, src, nullptr);
  lv_imgbtn_set_src(btn, LV_IMGBTN_STATE_PRESSED,  nullptr, src, nullptr);
  lv_imgbtn_set_src(btn, LV_IMGBTN_STATE_DISABLED, nullptr, src, nullptr);

  lv_obj_set_style_img_recolor(btn, lv_color_black(), LV_PART_MAIN | LV_STATE_PRESSED);
  lv_obj_set_style_img_recolor_opa(btn, LV_OPA_40, LV_PART_MAIN | LV_STATE_PRESSED);

  return btn;
}

void on_back(lv_event_t *) {
  Wait::close();
  if(wait_cb) wait_cb(CALLBACK_SOURCE::BACK);
}

void on_continue(lv_event_t *) {
  Wait::close();
  if(wait_cb) wait_cb(CALLBACK_SOURCE::CONTINUE);
}

void on_confirm(lv_event_t *event) {
  Wait::close();
  ExtUI::setUserConfirmed(false);
}

}
