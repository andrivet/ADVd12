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

#include "src/inc/MarlinConfigPre.h"
#include "tft.h"
#include "assets.h"
#include "src/inc/MarlinConfig.h"
#include "src/lcd/tft_io/tft_io.h"
#include "src/libs/W25Qxx.h"
#include <SPI.h>

void Tft::init() {
  TFT_IO::init();
  TFT_IO::initTFT();

  delay(100);
  draw_logo();
}

void Tft::clear(uint16_t color) {
  set_window(0, 0, TFT_WIDTH, TFT_HEIGHT);
  TFT_IO::writeMultiple(color, TFT_WIDTH * TFT_HEIGHT);
}

void Tft::draw_logo() {
#if HAS_LOGO_IN_FLASH
  uint8_t buffer[TFT_WIDTH * sizeof(uint16_t)];

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
  for(uint16_t y = 0; y < TFT_HEIGHT; y++) {
    W25QXXFlash::SPI_FLASH_BufferRead(buffer, ADDR_LOGO + offset, TFT_WIDTH * sizeof(uint16_t));
    TFT_IO::writeSequence(reinterpret_cast<uint16_t*>(buffer), TFT_WIDTH);
    offset += TFT_WIDTH * sizeof(uint16_t);
  }

  delay(2000);
#else
  clear(0);
#endif
}

void Tft::draw(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t * color_p) {
  set_window(x, y, width, height);
  TFT_IO::writeSequence(color_p, width * height);
}

void Tft::set_point(uint16_t x, uint16_t y, uint16_t point) {
  if(x >= TFT_WIDTH || y >= TFT_HEIGHT) return;
  set_window(x, y, 1, 1);
  TFT_IO::writeMultiple(point, 1);
}

void Tft::set_window(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  TFT_IO::set_window(x, y, x + width - 1, y + height - 1);
}
