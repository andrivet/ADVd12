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


#include "images.h"

#include <cassert>
#include <cstdint>

#include "assets.h"
#include "src/feature/powerloss.h"
#include "src/libs/W25Qxx.h"
#include "src/sd/cardreader.h"

struct __attribute__((__packed__)) Header {
  uint32_t magic;
  uint16_t width;
  uint16_t height;
};

struct DecodeContext {
  uint32_t *line_offsets;
  uint32_t address;
};

static constexpr uint32_t HEADER_MAGIC = 0x30454C52;

static lv_res_t decoder_info(lv_img_decoder_t * decoder, const void * src, lv_img_header_t * header);
static lv_res_t decoder_open(lv_img_decoder_t * decoder, lv_img_decoder_dsc_t * dsc);
static lv_res_t decoder_read_line(lv_img_decoder_t * decoder, lv_img_decoder_dsc_t * dsc, lv_coord_t x, lv_coord_t y, lv_coord_t len, uint8_t * buf);
static void decoder_close(lv_img_decoder_t * decoder, lv_img_decoder_dsc_t * dsc);


void Images::init() {
  auto img_decoder = lv_img_decoder_create();
  lv_img_decoder_set_info_cb(img_decoder, decoder_info);
  lv_img_decoder_set_open_cb(img_decoder, decoder_open);
  lv_img_decoder_set_read_line_cb(img_decoder, decoder_read_line);
  lv_img_decoder_set_close_cb(img_decoder, decoder_close);
}

static void dos_to_long(const uint8_t dos_name[11], char *long_name) {
  uint8_t j = 0;
  for(uint8_t i = 0; i < 11; ++i) {
    if(i == 8) long_name[j++] = '.';
    if(dos_name[i] == '\0' || dos_name[i] == ' ') continue;
    long_name[j++] = static_cast<char>(dos_name[i]);
  }
  long_name[j] = '\0';
}

void Images::update_assets() {
  if(!CardReader::isMounted()) return;
  MediaFile dir, root = CardReader::getroot();
  if(!dir.open(&root, "assets", O_RDONLY)) return;

  auto screen = lv_obj_create(nullptr);
  lv_scr_load(screen);

  auto label1 = lv_label_create(screen);
  lv_label_set_long_mode(label1, LV_LABEL_LONG_CLIP);
  lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(label1, LV_ALIGN_CENTER, 0, -12);

  auto label2 = lv_label_create(screen);
  lv_label_set_long_mode(label2, LV_LABEL_LONG_CLIP);
  lv_obj_set_style_text_align(label2, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(label2, LV_ALIGN_CENTER, 0, 12);

  lv_label_set_text(label1, "Erasing previous assets...");
  lv_refr_now(nullptr);
  W25QXX.init(SPI_QUARTER_SPEED);
  erase_assets(label2);

  uint32_t addresses[MAX_NB_IMAGES]{};
  uint32_t address = ADDR_IMAGES;

  lv_label_set_text(label1, "Loading assets...");
  for(dir_t entry; dir.readDir(&entry, CardReader::longFilename) > 0;) {
    if(CardReader::longFilename[0] == 0 && entry.name[0] != 0)
      dos_to_long(entry.name, CardReader::longFilename);
    if(CardReader::longFilename[0] == 0 || CardReader::longFilename[0] == '.') continue;

    lv_label_set_text(label2, CardReader::longFilename);
    lv_refr_now(nullptr);
    load_asset(dir, CardReader::longFilename, addresses, address);
  }

  lv_label_set_text(label1, "Write addresses...");
  lv_refr_now(nullptr);
  write_addresses(addresses);

  //dir.rename(&root, "_assets");
  dir.close();

  lv_label_set_text(label1, "Assets updated");
  lv_label_set_text(label2, "");
  lv_refr_now(nullptr);
}

void Images::erase_assets(lv_obj_t *label) {
  static constexpr unsigned NB_BLOCKS = SIZE_ASSETS / SIZE_BLOCK;
  char text[5];

  // Erase the whole memory by block because it is slow
  for(unsigned i = 0; i < NB_BLOCKS; ++i) {
    std::sprintf(text, "%u%%", (i + 1) * 100 / NB_BLOCKS);
    lv_label_set_text(label, text);
    lv_refr_now(nullptr);
    W25QXXFlash::SPI_FLASH_BlockErase(i * SIZE_BLOCK);
    MarlinHAL::watchdog_refresh();
  }

  // Write the signature
  uint32_t signature = SIGNATURE;
  W25QXXFlash::SPI_FLASH_BufferWrite(reinterpret_cast<uint8_t*>(&signature), ADDR_SIGNATURE, sizeof(signature));
}

bool parse_src(const char *src, uint32_t &index) {
  if(strncmp(src, "I:", 2) != 0) return false;
  index = strtoul(src + 2, nullptr, 10);
  return true;
}

bool Images::load_asset(MediaFile &dir, const char *name, uint32_t addresses[MAX_NB_IMAGES], uint32_t &address) {
  if(0 == strcasecmp(name, "LOGO.BIN")) {
    load_logo(dir);
    return false;
  }

  unsigned index = strtoul(name, nullptr, 10);
  assert(index < MAX_NB_IMAGES);
  addresses[index] = address;

  MediaFile file;
  if(!file.open(&dir, name, O_RDONLY)) {
    SERIAL_ECHOLNPGM("Error opening asset: ", name);
    return false;
  }

  uint8_t buffer[SPI_FLASH_PageSize];

  for(;;) {
    auto bytes = file.read(buffer, SPI_FLASH_PageSize);
    if(bytes <= 0) break;
    // We always write a full page
    W25QXXFlash::SPI_FLASH_BufferWrite(buffer, address, SPI_FLASH_PageSize);
    address += SPI_FLASH_PageSize; // address will always be page-aligned
    if(bytes < SPI_FLASH_PageSize) break;
  }

  file.close();
  return true;
}

bool Images::load_logo(MediaFile &dir) {
  MediaFile file;
  if(!file.open(&dir, "logo.bin", O_RDONLY)) {
    SERIAL_ECHOLNPGM("Error opening asset: logo.bin");
    return false;
  }

  uint8_t buffer[SPI_FLASH_PageSize];
  uint32_t address = ADDR_LOGO;
  for(;;) {
    auto bytes = file.read(buffer, sizeof(buffer));
    if(bytes <= 0) break;
    W25QXXFlash::SPI_FLASH_BufferWrite(buffer, address, bytes);
    address += bytes;
  }

  file.close();
  return true;
}

void Images::write_addresses(uint32_t addresses[MAX_NB_IMAGES]) {
  W25QXXFlash::SPI_FLASH_BufferWrite(reinterpret_cast<uint8_t*>(addresses), ADDR_ADDRS, MAX_NB_IMAGES * sizeof(uint32_t));
}

static lv_res_t decoder_info(lv_img_decoder_t * decoder, const void * src, lv_img_header_t * header) {
  if(lv_img_src_get_type(src) != LV_IMG_SRC_FILE) return LV_RES_INV;

  uint32_t index;
  if(!parse_src(static_cast<const char*>(src), index)) return LV_RES_INV;

  uint32_t address{};
  W25QXXFlash::SPI_FLASH_BufferRead(reinterpret_cast<uint8_t*>(&address), ADDR_ADDRS + index * sizeof(uint32_t), sizeof(address));

  Header hdr{};
  W25QXXFlash::SPI_FLASH_BufferRead(reinterpret_cast<uint8_t*>(&hdr), address, sizeof(Header));
  if(hdr.magic != HEADER_MAGIC) return LV_RES_INV;

  header->always_zero = 0;
  header->w = hdr.width;
  header->h = hdr.height;
  header->cf = LV_IMG_CF_RGB565;

  return LV_RES_OK;
}

static lv_res_t decoder_open(lv_img_decoder_t * decoder, lv_img_decoder_dsc_t * dsc) {
  if(lv_img_src_get_type(dsc->src) != LV_IMG_SRC_FILE) return LV_RES_INV;

  uint32_t index;
  if(!parse_src(static_cast<const char*>(dsc->src), index)) return LV_RES_INV;

  uint32_t address{};
  W25QXXFlash::SPI_FLASH_BufferRead(reinterpret_cast<uint8_t*>(&address), ADDR_ADDRS + index * sizeof(uint32_t), sizeof(address));

  Header hdr{};
  W25QXXFlash::SPI_FLASH_BufferRead(reinterpret_cast<uint8_t*>(&hdr), address, sizeof(Header));
  if(hdr.magic != HEADER_MAGIC) return LV_RES_INV;

  auto line_offsets_size = hdr.height * sizeof(uint32_t);
  auto line_offsets = static_cast<uint32_t*>(lv_mem_alloc(line_offsets_size));
  if(!line_offsets) return LV_RES_INV;

  W25QXXFlash::SPI_FLASH_BufferRead(reinterpret_cast<uint8_t*>(line_offsets), address + sizeof(Header), line_offsets_size);

  auto ctx = static_cast<DecodeContext*>(lv_mem_alloc(sizeof(DecodeContext)));
  if(!ctx) {
    lv_mem_free(line_offsets);
    return LV_RES_INV;
  }

  ctx->line_offsets = line_offsets;
  ctx->address = address;

  dsc->user_data = ctx;
  dsc->img_data = nullptr;

  return LV_RES_OK;
}

static void decode_line(uint8_t *line_buffer, uint32_t line_addr, uint16_t width) {
  uint8_t run[3];

  for(uint16_t x = 0; x < width; ) {
    W25QXXFlash::SPI_FLASH_BufferRead(run, line_addr, sizeof(run));
    line_addr += sizeof(run);

    auto count = run[0];
    auto px = static_cast<uint16_t>(run[1]) | static_cast<uint16_t>(run[2]) << 8;

    if(count == 0 || static_cast<uint32_t>(x) + count > width) return;

    auto dst16 = reinterpret_cast<uint16_t*>(line_buffer);
    for(uint8_t i = 0; i < count; i++)
      dst16[x++] = px;
  }
}

static lv_res_t decoder_read_line(lv_img_decoder_t * decoder, lv_img_decoder_dsc_t * dsc, lv_coord_t x, lv_coord_t y, lv_coord_t len, uint8_t * buf) {
  auto ctx = static_cast<DecodeContext*>(dsc->user_data);
  if(!ctx) return LV_RES_INV;

  if(
    y < 0 || y >= dsc->header.h ||
    x < 0 || x >= dsc->header.w ||
    len <= 0 || x + len > dsc->header.w
  )
    return LV_RES_INV;

  auto line_offsets_size = dsc->header.h * sizeof(uint32_t);
  auto line_address = ctx->address + sizeof(Header) + line_offsets_size + ctx->line_offsets[y];
  auto line_buffer = static_cast<uint8_t*>(lv_mem_alloc(dsc->header.w * sizeof(uint16_t)));
  decode_line(line_buffer, line_address, dsc->header.w);
  lv_memcpy(buf, line_buffer + x * sizeof(uint16_t), len * sizeof(uint16_t));
  lv_mem_free(line_buffer);

  return LV_RES_OK;
}

static void decoder_close(lv_img_decoder_t * decoder, lv_img_decoder_dsc_t * dsc) {
  auto ctx = static_cast<DecodeContext*>(dsc->user_data);
  if(!ctx) return;

  if(ctx->line_offsets) lv_mem_free(ctx->line_offsets);
  lv_mem_free(ctx);

  dsc->user_data = nullptr;
}
