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
#include "core.h"
#include "task.h"
#include "status.h"
#include "display.h"
#include "reentrant.h"
#include "progress.h"
#include "dates.h"
#include "../panels/panels.h"
#include "../panels/print/print.h"
#include "../panels/main.h"

namespace {

constexpr unsigned int Z_ROOM = 20; // mm

char filename[64];
bool usb_print = true;

void send_gplv3_7b_notice();

}

void advd12::Core::init() {
  send_gplv3_7b_notice(); // You are not authorized to remove or alter this notice
  Display::init();
  Status::init();
  Progress::reset();

#if HAS_LEVELING
  if(ExtUI::getLevelingIsValid()) ExtUI::setLevelingActive(true);
#endif

  Status::set(GET_TEXT(WELCOME_MSG), Status::STATUS_OPTIONS::RESET);
}

void advd12::Core::startup() {
  Status::set(GET_TEXT(ADVD12_MSG_BOOTING), Status::STATUS_OPTIONS::RESET | Status::STATUS_OPTIONS::PERSISTENT);
}

void advd12::Core::idle() {
  static Reentrant reentrant;
  ReentrantScope scope{reentrant};

  if(!scope.reentrant()) {
    PanelBase::update_variables();
    background_task.execute();
    status_task.execute();
    wait_task.execute();
  }
}

float advd12::Core::ensure_z_enough_room() {
  auto previous_z = ExtUI::getAxisPosition_mm(ExtUI::Z);
  // If the Z position is low and the Z axis position is known, raise it
  if(previous_z < 20 && ExtUI::isAxisPositionKnown(ExtUI::Z))
    ExtUI::setAxisPosition_mm(Z_ROOM, ExtUI::Z, 20);
  return previous_z;
}

bool advd12::Core::is_busy() {
  return
      ExtUI::isPrinting() ||
      ExtUI::isMoving() ||
      ExtUI::getHostKeepaliveState() == GcodeSuite::MarlinBusyState::IN_HANDLER ||
      ExtUI::getHostKeepaliveState() == GcodeSuite::MarlinBusyState::IN_PROCESS;
}

void advd12::Core::task_handler() {
  Display::task_handler();
}

void advd12::Core::calibrate() {
  Display::calibrate();
}

void advd12::Core::disable_motors() {
  // If busy, do nothing
  if(is_busy()) {
    Status::set(GET_TEXT(ADVD12_MSG_BUSY));
    return;
  }

  ExtUI::stopMove(); // Warning: will call idle() and thus is reentrant
  ExtUI::injectCommands_P("M84"); // Disable steppers
  ExtUI::setAllAxisUnhomed();
  ExtUI::setAllAxisPositionUnknown();
  Status::set(GET_TEXT(ADVD12_MSG_MOTORS_DISABLED), Status::STATUS_OPTIONS::RESET);
}

const char *advd12::Core::version() {
  static constexpr char version[] = {
    '0' + ADVD12_VERSION / 0x0100,
    '.',
    '0' + (ADVD12_VERSION % 0x0100) / 0x10,
    '.',
    '0' + (ADVD12_VERSION % 0x10),
    0
  };
  return version;
}

const char *advd12::Core::build() {
  static constexpr char build[] = {
    '0' + (YEAR__ - 2000) / 10,
    '0' + (YEAR__ - 2000) % 10,
    '0' + MONTH__ / 10,
    '0' + MONTH__ % 10,
    '0' + DAY__ / 10,
    '0' + DAY__ % 10,
    '0' + HOUR__ / 10,
    '0' + HOUR__ % 10,
    '0' + MIN__ / 10,
    '0' + MIN__ % 10,
    '0' + SEC__ / 10,
    '0' + SEC__ % 10,
    0
  };
  return build;
}

void advd12::Core::print_started() {
  // Warning: it is called 2 times
  Progress::start();
  switch_panel<PrintPanel>();
}

void advd12::Core::print_done() {
  Progress::finish();
  switch_panel<MainPanel>();
  usb_print = true;
  Status::set_done();
}

const char *advd12::Core::print_filename() {
  return usb_print ? "USB" : filename;
}

void advd12::Core::set_print_filename(const char *name) {
  auto length = strlen(name);
  if(length >= sizeof(filename))
    length = sizeof(filename) - 1;
  memcpy(filename, name, length);
  usb_print = false;
}

namespace {

void send_gplv3_7b_notice() {
  SERIAL_ECHO_START();
  //! Note to forks author:
  //! Under GPLv3 provision 7(b), you are not authorized to remove or alter this notice.
  SERIAL_ECHOLNPGM("Based on ADVd12, Copyright (C) 2026 Sebastien Andrivet");
  SERIAL_ECHO_START();
  SERIAL_ECHOLNPGM("ADVd12 version: ", advd12::Core::version(), ", build: ", advd12::Core::build(), ", flavor: ", F(ADVD12_NAME));
}

}
