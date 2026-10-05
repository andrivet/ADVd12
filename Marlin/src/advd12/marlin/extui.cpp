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
#include "../core/core.h"
#include "../core/status.h"
#include "../panels/wait.h"
#include "../panels/kill.h"
#include "../core/progress.h"
#include "src/advd12/panels/leveling/automatic.h"

namespace ExtUI {

  using namespace advd12;

  void onStartup() {
  }

  void onIdle() {
    Core::idle();
  }

  void onMediaMounted() {
    //SdCard::on_media_inserted();
  }

  void onMediaError() {
    //SdCard::on_media_error();
  }

  void onMediaRemoved() {
    //SdCard::on_media_removed();
  }

  void onMediaOpenError(const char*) {
    //SdCard::on_media_open_error();
  }

  void onHeatingError(const heater_id_t header_id) {
    // Nothing to do, the printer will call onPrinterKilled just after
  }

  void onMinTempError(const heater_id_t header_id) {
    // Nothing to do, the printer will call onPrinterKilled just after
  }

  void onMaxTempError(const heater_id_t header_id) {
    // Nothing to do, the printer will call onPrinterKilled just after
  }

  void onPlayTone(const uint16_t frequency, const uint16_t duration) {
    //Buzzer::buzz_on_action(duration);
  }

  void onPrinterKilled(FSTR_P const error, FSTR_P const component) {
    Kill::killed(FTOP(error), FTOP(component));
  }

  void onPrintTimerStarted() {
    // Warning: it is called 2 times
    Core::print_started();
  }

  void onPrintTimerPaused() {
    // Nothing to do;
  }

  void onPrintTimerStopped() {
    Core::print_done();
  }

  void onPrintDone() {
    // Nothing to do
  }

  void onFilamentRunout(const extruder_t extruder) {
    // TODO
  }

  void onUserConfirmRequired(const char * const msg) {
    const auto awaiting = ExtUI::awaitingUserConfirm();
    Wait::wait_user(GET_TEXT(ADVD12_TITLE_CONFIRM), msg, awaiting);
  }

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  void onPauseMode(
      const PauseMessage message,
      const PauseMode mode,
      const uint8_t extruder
  ) {
    if (mode != PAUSE_MODE_SAME) pause_mode = mode;
    ExtUI::pauseModeStatus = message;
    auto title = GET_TEXT(ADVD12_MSG_TITLE_PAUSE);
    switch (message) {
      case PAUSE_MESSAGE_CHANGING: Wait::wait_user(title, GET_TEXT(MSG_FILAMENT_CHANGE_INIT), false); break;
      case PAUSE_MESSAGE_HEATING:  Wait::wait_user(title, GET_TEXT(MSG_FILAMENT_CHANGE_HEATING), false); break;
      case PAUSE_MESSAGE_UNLOAD:   Wait::wait_user(title, GET_TEXT(MSG_FILAMENT_CHANGE_UNLOAD), false); break;
      case PAUSE_MESSAGE_INSERT:   Wait::wait_user(title, GET_TEXT(MSG_FILAMENT_CHANGE_INSERT), false); break;
      case PAUSE_MESSAGE_HEAT:     Wait::wait_user(title, GET_TEXT(MSG_FILAMENT_CHANGE_HEAT), false); break;
      case PAUSE_MESSAGE_LOAD:     Wait::wait_user(title, GET_TEXT(MSG_FILAMENT_CHANGE_LOAD), false); break;
      case PAUSE_MESSAGE_PURGE:    Wait::wait_user(title, GET_TEXT(MSG_FILAMENT_CHANGE_PURGE), false); break;
      case PAUSE_MESSAGE_OPTION:   Wait::pause_options(); break;
      case PAUSE_MESSAGE_RESUME:   Wait::wait_user(title, GET_TEXT(MSG_FILAMENT_CHANGE_RESUME), false); break;
      case PAUSE_MESSAGE_STATUS:   Wait::close(); break;

      case PAUSE_MESSAGE_PARKING:  Wait::wait_user(title, GET_TEXT(MSG_PAUSE_PRINT_PARKING), false); ExtUI::setUserConfirmed(false); break;
      case PAUSE_MESSAGE_WAITING:  break;
      default: break;
    }
  }
#endif

  void onShowStatus() {
  }

  void onStatusChanged(const char * const msg) {
    //Status::set(msg, persist ? Status::STATUS_OPTIONS::PERSISTENT | Status::STATUS_OPTIONS::RESET : Status::STATUS_OPTIONS::RESET );
    //if(persist) Dimming::sleep_off();
  }

  void onHomingStart() {
    Wait::homing_start();
  }

  void onHomingDone() {
    Wait::homing_done();
  }

  void onFactoryReset() {
  }

  void onStoreSettings(char *buff) {
  }

  void onLoadSettings(const char *buff) {
  }

  void onPostprocessSettings() {
    // Called after loading or resetting stored settings
  }

  void onSettingsStored(bool success) {
    // Called after the entire EEPROM has been written,
    // whether successful or not.
  }

  void onSettingsLoaded(bool success) {
    // Called after the entire EEPROM has been read,
    // whether successful or not.
    //Core::on_settings_loaded(success);
  }

  void onSettingsValidated(bool success) {
    //Core::on_settings_validated(success);
  }

  void onLevelingStart() {
#ifdef BLTOUCH
    AutomaticPanel::on_leveling_start();
#endif
  }

  void onLevelingProgress(const uint8_t index, const int8_t xpos, const int8_t ypos) {
#ifdef BLTOUCH
    AutomaticPanel::on_leveling_progress(index);
#endif
  }

  void onLevelingDone(bool success) {
#ifdef BLTOUCH
    AutomaticPanel::on_leveling_done(success);
#endif
  }

  void onMeshUpdate(const int8_t xpos, const int8_t ypos, const float zval) {
    // Called when any mesh point is updated
  }

  #if ENABLED(PREVENT_COLD_EXTRUSION)
  void onSetMinExtrusionTemp(const celsius_t) {
  }
  #endif

#if ENABLED(POWER_LOSS_RECOVERY)
  void onSetPowerLoss(const bool onoff) {
    //Core::on_power_loss_set(onoff);
  }

  void onPowerLoss() {
    //Core::on_power_loss();
  }

void onPowerLossResume() {
    // Called on resume from power-loss
    //Core::on_power_loss_resume();
  }
#endif

  void onPIDTuning(const pidresult_t rst) {
    /*if(rst == PID_STARTED || rst == PID_BED_STARTED)
      PidTuning::on_start(rst == PID_BED_STARTED);
    else
      PidTuning::on_finished(rst);*/
  }

  void onStartM303(const int count, const heater_id_t hid, const celsius_t temp) {
  }

  void onPIDTuningProgress(int cycleIndex, int nbCycles) {
    //PidTuning::on_progress(cycleIndex, nbCycles);
  }

  void onPIDTuningReportTemp(int /*heater*/) {
    // Nothing to do
  }

  void onSteppersDisabled() {
  }

  void onSteppersEnabled() {
  }

  void onAxisDisabled(const axis_t axis) {
  }

  void onAxisEnabled(const axis_t axis) {
  }

  #if ENABLED(PLATFORM_M997_SUPPORT)
  void onFirmwareFlash() {}
  #endif

}
