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
#include <lvgl.h>
#include "display.h"
#include "assets.h"
#include "images.h"
#include "../panels/main.h"
#include "../panels/panels.h"
#include "../panels/calibration.h"
#include "../../HAL/HAL.h"
#include "../../lcd/marlinui.h"
#include "../../lcd/tft_io/tft_io.h"
#include "../../libs/W25Qxx.h"

#include HAL_PATH(../../.., tft/xpt2046.h)

namespace {
  constexpr uint8_t NB_LINES = 8; // Takes 960 bytes per line

  void init_lvgl();
  void init_lvgl_drivers();
  void flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p);
  void input_read(lv_indev_drv_t * indev, lv_indev_data_t * data);
  void systick();
  void clear(uint16_t color);
  void draw_logo();
  void draw(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t *color_p);
  void draw_dma(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t *color_p, void(*cb)());
  void set_point(uint16_t x, uint16_t y, uint16_t point);
  void set_window(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

  lv_disp_drv_t disp_drv;
  lv_indev_drv_t indev_drv;
  lv_disp_draw_buf_t draw_buffer;
  lv_color_t draw_data[TFT_WIDTH * NB_LINES];

}

void Display::init() {
  W25QXX.init(SPI_QUARTER_SPEED);

  MarlinHAL::watchdog_refresh();
  TFT_IO::init();
  TFT_IO::initTFT();
  delay(100);
  draw_logo();
  MarlinHAL::watchdog_refresh();

  XPT2046::init();
  init_lvgl();

  Images::update_assets();
  MarlinHAL::watchdog_refresh();

  // TODO: init runout
  // TODO: init Wi-Fi
  // TODO: init servo and probe

  if(TouchCalibration::need_calibration())
    CalibrationPanel::create();
  else
    MainPanel::create();
}

void Display::task_handler() {
  lv_timer_handler();
}

void Display::calibrate() {
  switch_panel<CalibrationPanel>();
}

namespace {

  void init_lvgl() {
    lv_init();
    lv_disp_draw_buf_init(&draw_buffer, draw_data, nullptr, TFT_WIDTH * NB_LINES);

    init_lvgl_drivers();
    Images::init();

    systick_attach_callback(systick);

    Display::init_lvgl_styles();
  }

  void init_lvgl_drivers() {
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = flush;
    disp_drv.draw_buf = &draw_buffer;
    disp_drv.hor_res = TFT_WIDTH;
    disp_drv.ver_res = TFT_HEIGHT;
    lv_disp_drv_register(&disp_drv);

    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = input_read;
    lv_indev_drv_register(&indev_drv);
  }

  void systick() {
    lv_tick_inc(1);

    // TODO: update print time
    // TODO: filament loading time
  }

  void flush_ready_callback() {
    lv_disp_flush_ready(&disp_drv);
  }

  void flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    auto width = area->x2 - area->x1 + 1;
    auto height = area->y2 - area->y1 + 1;

    draw_dma(
      area->x1,
      area->y1,
      width,
      height,
      reinterpret_cast<uint16_t*>(color_p),
      flush_ready_callback
    );
  }

  bool get_touch(int16_t * const x, int16_t * const y) {
    if(!XPT2046::getRawPoint(x, y)) return false;

    auto state = TouchCalibration::get_calibration_state();
    if(WITHIN(state, CALIBRATION_TOP_LEFT, CALIBRATION_BOTTOM_LEFT)) {
      if(TouchCalibration::handleTouch(*x, *y)) CalibrationPanel::update();
      return false;
    }

    *x = static_cast<int16_t>((uint32_t(*x) * _TOUCH_CALIBRATION_X) >> 16) + _TOUCH_OFFSET_X;
    *y = static_cast<int16_t>((uint32_t(*y) * _TOUCH_CALIBRATION_Y) >> 16) + _TOUCH_OFFSET_Y;

    return true;
  }

  void input_read(lv_indev_drv_t *indev, lv_indev_data_t *data) {
    static xy_int_t last {};
    data->state = get_touch(&last.x, &last.y) ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
    data->point.x = last.x;
    data->point.y = last.y;
  }

  void clear(uint16_t color) {
    set_window(0, 0, TFT_WIDTH, TFT_HEIGHT);
    TFT_IO::writeMultiple(color, TFT_WIDTH * TFT_HEIGHT);
  }

  void draw_logo() {
#if HAS_LOGO_IN_FLASH
    W25QXX.init(SPI_QUARTER_SPEED);

    // Be sure there is a logo, do not display garbage (from a previous firmware)
    uint32_t signature = 0;
    W25QXXFlash::SPI_FLASH_BufferRead(reinterpret_cast<uint8_t*>(&signature), ADDR_SIGNATURE, sizeof(signature));
    if(signature != SIGNATURE) {
      clear(0);
      return;
    }

    set_window(0, 0, TFT_WIDTH, TFT_HEIGHT);

    uint32_t offset = 0;
    for(uint16_t y = 0; y < TFT_HEIGHT; y += NB_LINES) {
      auto lines = std::min(static_cast<int>(NB_LINES), TFT_HEIGHT - y);
      auto pixels = TFT_WIDTH * lines;
      auto bytes = pixels * sizeof(lv_color_t);
      W25QXXFlash::SPI_FLASH_BufferRead(reinterpret_cast<uint8_t*>(draw_data), ADDR_LOGO + offset, bytes);
      TFT_IO::writeSequence(reinterpret_cast<uint16_t*>(draw_data), pixels);
      offset += bytes;
    }

    delay(2000);
#else
    clear(0);
#endif
  }

  [[maybe_unused]] void draw(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t * color_p) {
    set_window(x, y, width, height);
    TFT_IO::writeSequence(color_p, width * height);
  }

  void draw_dma(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t * color_p, void(*cb)()) {
    set_window(x, y, width, height);
    TFT_IO::writeSequenceIT(color_p, width * height, cb);
  }

  [[maybe_unused]] void set_point(uint16_t x, uint16_t y, uint16_t point) {
    if(x >= TFT_WIDTH || y >= TFT_HEIGHT) return;
    set_window(x, y, 1, 1);
    TFT_IO::writeMultiple(point, 1);
  }

  void set_window(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    TFT_IO::set_window(x, y, x + width - 1, y + height - 1);
  }


}
