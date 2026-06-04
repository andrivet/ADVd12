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

namespace ExtUI {

  using namespace advd12;

  void onStartup() {
    //Core::startup();
  }

  void onIdle() {
    //Core::idle();
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
    //Core::killed(GET_TEXT_F(MSG_ERR_HEATING_FAILED), header_id);
  }

  void onMinTempError(const heater_id_t header_id) {
    //Core::killed(GET_TEXT_F(MSG_ERR_MINTEMP), header_id);
  }

  void onMaxTempError(const heater_id_t header_id) {
    //Core::killed(GET_TEXT_F(MSG_ERR_MAXTEMP), header_id);
  }

  void onPlayTone(const uint16_t frequency, const uint16_t duration) {
    //Buzzer::buzz_on_action(duration);
  }

  void onPrinterKilled(FSTR_P const error, FSTR_P const component) {
    //Core::killed(error, component);
  }

  void onPrintTimerStarted() {
    //Core::print_started();
  }

  void onPrintTimerPaused() {
    //Core::print_paused();
  }

  void onPrintTimerStopped() {
    //Core::print_done();
  }

  void onPrintDone() {
    //Core::print_done();
  }

  void onFilamentRunout(const extruder_t extruder) {
  }

  void onUserConfirmRequired(const char * const msg) {
    //const auto awaiting = ExtUI::awaitingUserConfirm();
    //Wait::wait_user(GET_TEXT_F(ADVI3PP_TITLE_CONFIRM), msg, awaiting);
  }

  void onChangeFilamentConfirmRequired() {
    //Core::pause_temperature();
  }

  void onPauseMode(
      const PauseMessage message,
      const PauseMode mode,
      const uint8_t extruder
  ) {
    stdOnPauseMode(message, mode, extruder);
  }

  void onShowStatus() {
  }

  void onStatusChanged(const char * const msg) {
    //Status::set(msg, persist ? Status::STATUS_OPTIONS::PERSISTENT | Status::STATUS_OPTIONS::RESET : Status::STATUS_OPTIONS::RESET );
    //if(persist) Dimming::sleep_off();
  }

  void onHomingStart() {
    //Wait::homing_start();
  }

  void onHomingDone() {
    //Wait::homing_done();
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
    //Mesh::start();
  }

  void onLevelingProgress(const int8_t index, const int8_t xpos, const int8_t ypos) {
    //Mesh::progress(index);
  }

  void onLevelingDone(bool success) {
    //Mesh::done(success);
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
