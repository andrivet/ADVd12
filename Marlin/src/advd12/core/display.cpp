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
#include "tft.h"
#include "images.h"
#include "../panels/main.h"
#include "../panels/calibration.h"

#include "../../HAL/HAL.h"
#include "../../lcd/marlinui.h"
#include "../../libs/W25Qxx.h"

#include HAL_PATH(../../.., tft/xpt2046.h)

namespace advd12::Display {
namespace {

  void init_lvgl();
  void init_lvgl_drivers();
  void flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p);
  void input_read(lv_indev_drv_t * indev, lv_indev_data_t * data);
  void systick();

  lv_disp_drv_t disp_drv;
  lv_indev_drv_t indev_drv;
  lv_disp_draw_buf_t draw_buffer;
  lv_color_t draw_data[TFT_WIDTH * 16];

}

void init() {
  W25QXX.init(SPI_QUARTER_SPEED);

  MarlinHAL::watchdog_refresh();
  Tft::init();
  MarlinHAL::watchdog_refresh();

  XPT2046::init();
  init_lvgl();

  Images::update_assets();
  MarlinHAL::watchdog_refresh();

  // TODO: init runout
  // TODO: init Wi-Fi
  // TODO: init servo and probe

  // TODO: draw main screen
  if(TouchCalibration::need_calibration())
    calibration_panel.show();
  else
    main_panel.show();
}

void task_handler() {
  // TODO: DMA
  lv_timer_handler();
}

void draw_error_message(FSTR_P msg) {
  // TODO
}

void calibrate() {
  Panel::switch_panel(calibration_panel);
}

namespace {

  void init_lvgl() {
    lv_init();
    lv_disp_draw_buf_init(&draw_buffer, draw_data, nullptr, sizeof(draw_data) / sizeof(uint16_t));

    init_lvgl_drivers();
    Images::init();

    systick_attach_callback(systick);

    init_lvgl_styles();
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

  void flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    auto width = area->x2 - area->x1 + 1;
    auto height = area->y2 - area->y1 + 1;

    Tft::draw(area->x1, area->y1, width, height, reinterpret_cast<uint16_t*>(color_p));
    lv_disp_flush_ready(disp);
  }

  bool get_touch(int16_t * const x, int16_t * const y) {
    if(!XPT2046::getRawPoint(x, y)) return false;

    auto state = TouchCalibration::get_calibration_state();
    if(WITHIN(state, CALIBRATION_TOP_LEFT, CALIBRATION_BOTTOM_LEFT)) {
      if(TouchCalibration::handleTouch(*x, *y)) calibration_panel.update();
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

}
}
