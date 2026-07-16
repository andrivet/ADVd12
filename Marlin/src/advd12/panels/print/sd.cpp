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
#include "../../../inc/MarlinConfig.h"
#include "../../../lcd/extui/ui_api.h"
#include "sd.h"
#include "print.h"
#include "../panels.h"
#include "../main.h"
#include "../../core/status.h"
#include "../../core/styles.h"
#include "../../core/ids.h"
#include "src/advd12/panels/wait.h"

namespace {

constexpr uint16_t NB_VISIBLE_SD_FILES = 6; //!< Number of files per page on the SD screen
enum class FileType { None, File, Folder };
constexpr const char *IMG[] = { IMG_FS_EMPTY, IMG_FS_FILE, IMG_FS_FOLDER };
constexpr uint16_t ROW_WIDTH = 288;
constexpr uint16_t ROW_HEIGHT = 37;

unsigned page_ = 0;
ExtUI::FileList files_{};
lv_obj_t *icons_[NB_VISIBLE_SD_FILES] = {};
lv_obj_t *filenames_[NB_VISIBLE_SD_FILES] = {};
lv_obj_t *rows_[NB_VISIBLE_SD_FILES] = {};
lv_obj_t *nb_pages_ = nullptr;

Panel<10> panel;

lv_obj_t *icon(lv_coord_t x, lv_coord_t y);
lv_obj_t *line(lv_coord_t y);
lv_obj_t *row(lv_coord_t y);
void show_first_page();
void show_current_page();
const char *get_file_name(uint8_t index_in_page, FileType &type);
void select_row(unsigned index);

void on_back(lv_event_t *e);
void on_up(lv_event_t *e);
void on_down(lv_event_t *e);
void on_parent(lv_event_t *e);
void on_file0(lv_event_t *e);
void on_file1(lv_event_t *e);
void on_file2(lv_event_t *e);
void on_file3(lv_event_t *e);
void on_file4(lv_event_t *e);
void on_file5(lv_event_t *e);

}

bool SdPanel::can_switch() {
  return Wait::check_not_busy();
}

void SdPanel::create() {
  if(!panel.create()) return;
  panel.layout_menu("microSD Card");

  panel.rectangle(ROW_WIDTH, 56, 3, 228);

  auto back = panel.control(IMG_LEFT, 7, 2);
  auto parent = panel.circle_button(292, 59, "..");
  auto up = panel.control(IMG_UP, 292, 114);
  auto down = panel.control(IMG_DOWN, 292, 230);

  for(uint8_t index = 0; index < NB_VISIBLE_SD_FILES; ++index) {
    icons_[index] = icon(3, 59 + index * ROW_HEIGHT);
    filenames_[index] = panel.text("", 45, 66 + index * ROW_HEIGHT, 236, ALIGN::LEFT, LABEL::REGULAR);
    rows_[index] = row(58 + index * ROW_HEIGHT);

    if(index < NB_VISIBLE_SD_FILES - 1) line(94 + index * ROW_HEIGHT);
  }

  nb_pages_ = panel.text("1/1", 290, 185, 53, ALIGN::CENTER, LABEL::REGULAR);

  panel.reset_bindings();
  panel.bind(back, &on_back);
  panel.bind(parent, &on_parent);
  panel.bind(up, &on_up);
  panel.bind(down, &on_down);
  panel.bind(rows_[0], &on_file0);
  panel.bind(rows_[1], &on_file1);
  panel.bind(rows_[2], &on_file2);
  panel.bind(rows_[3], &on_file3);
  panel.bind(rows_[4], &on_file4);
  panel.bind(rows_[5], &on_file5);

  ExtUI::mountMedia();
  if(!ExtUI::isMediaMounted()) return;

  Status::set(GET_TEXT(ADVD12_MSG_SD_CARD_DETECTED), Status::STATUS_OPTIONS::RESET);
  show_first_page();
}

namespace {

void on_back(lv_event_t *e) {
  switch_panel<MainPanel>();
}

void on_up(lv_event_t *e) {
  if(!ExtUI::isMediaMounted()) return;
  if(page_ <= 0) return;
  page_ -= 1;
  show_current_page();
}

void on_down(lv_event_t *e) {
  if(!ExtUI::isMediaMounted()) return;
  if(files_.count() <= page_ * NB_VISIBLE_SD_FILES + NB_VISIBLE_SD_FILES) return;
  page_ += 1;
  show_current_page();
}

void on_parent(lv_event_t *e) {
  if(!ExtUI::isMediaMounted() || files_.isAtRootDir()) return;
  files_.upDir();
  files_.refresh();
  show_first_page();
}

void on_file0(lv_event_t *e) {
  select_row(0);
}

void on_file1(lv_event_t *e) {
  select_row(1);
}

void on_file2(lv_event_t *e) {
  select_row(2);
}

void on_file3(lv_event_t *e) {
  select_row(3);
}

void on_file4(lv_event_t *e) {
  select_row(4);
}

void on_file5(lv_event_t *e) {
  select_row(5);
}

lv_obj_t *icon(lv_coord_t x, lv_coord_t y) {
  return panel.icon(IMG_FS_EMPTY, x, y);
}

lv_obj_t *line(lv_coord_t y) {
  auto rect = lv_obj_create(panel.scr());
  lv_obj_set_pos(rect, 0, y);
  lv_obj_set_size(rect, ROW_WIDTH, 1);
  lv_obj_set_style_bg_color(rect, lv_color_make(128, 128, 128), 0);
  lv_obj_set_style_bg_opa(rect, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(rect, 0, 0);
  lv_obj_set_style_radius(rect, 0, 0);
  lv_obj_set_style_pad_all(rect, 0, 0);
  lv_obj_clear_flag(rect, LV_OBJ_FLAG_SCROLLABLE);
  return rect;
}

lv_obj_t *row(lv_coord_t y) {
  auto obj = lv_obj_create(panel.scr());
  lv_obj_set_pos(obj, 0, y);
  lv_obj_set_size(obj, ROW_WIDTH, ROW_HEIGHT - 2);
  lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, 0);
  lv_obj_set_style_bg_color(obj, lv_color_black(), LV_STATE_PRESSED);
  lv_obj_set_style_bg_opa(obj, LV_OPA_30, LV_STATE_PRESSED);
  lv_obj_set_style_border_width(obj, 0, 0);
  lv_obj_set_style_radius(obj, 0, 0);
  lv_obj_set_style_pad_all(obj, 0, 0);
  return obj;
}

void show_first_page() {
  page_ = 0;
  show_current_page();
}

void show_current_page() {
  FileType file_type = FileType::None;

  for(uint8_t index = 0; index < NB_VISIBLE_SD_FILES; ++index) {
    auto file_name = get_file_name(index, file_type);
    auto img = IMG[static_cast<unsigned>(file_type)];
    lv_img_set_src(icons_[index], img);
    lv_label_set_text(filenames_[index], file_name);
  }

  auto nb_pages = (files_.count() + NB_VISIBLE_SD_FILES - 1) / NB_VISIBLE_SD_FILES;
  char text[8];
  snprintf(text, sizeof(text), "%d/%d", page_ + 1, nb_pages <= 0 ? 0 : nb_pages);
  lv_label_set_text(nb_pages_, text);
}

const char *get_file_name(uint8_t index_in_page, FileType &type) {
  type = FileType::None;

  auto absolute_index = index_in_page + page_ * NB_VISIBLE_SD_FILES;
  if(absolute_index < files_.count()) {
    ExtUI::FileList files{};
    files.seek(absolute_index);
    type = files.isDir() ? FileType::Folder : FileType::File;
    return files.filename();
  }

  return "";
}

void select_row(unsigned index) {
  if(!ExtUI::isMediaMounted()) return;

  auto absolute_index = index + page_ * NB_VISIBLE_SD_FILES;
  if(absolute_index >= files_.count()) return;
  files_.seek(absolute_index);

  const char *filename = files_.shortFilename();
  if(!filename) {
    ExtUI::onMediaOpenError(nullptr);
    return;
  }

  if(files_.isDir()) {
    files_.changeDir(filename);
    files_.refresh();
    show_first_page();
    return;
  }

  char text[40];
  snprintf(text, sizeof(text), "Print file %s?", files_.filename());
  Wait::wait_back_continue("Print", text, [](CALLBACK_SOURCE src) {
    if(src == CALLBACK_SOURCE::BACK) return;
    ExtUI::printFile(files_.shortFilename());
    PrintPanel::set_filename(files_.filename());
    switch_panel<PrintPanel>();
  });
}

}
