#include "interface.h"

#include <algorithm>
#include <iostream>

Interface::Interface() {}

Interface::~Interface() {}

void Interface::Show() {
  std::cout << "===== Huffman compression algorithm =====" << std::endl;
  while (m_run) {
    try {
      ScanInput();
    } catch (const std::exception& e) {
      std::cout << "> " << e.what() << std::endl;
    }
  }
  std::cout << "bye!" << std::endl;
}

void Interface::ScanInput() {
  std::string word{};
  std::cin >> word;
  std::transform(word.begin(), word.end(), word.begin(), ::tolower);
  if (word == "ar") {
    std::string dir1{}, dir2{};
    std::cin >> word >> dir1 >> dir2;
    SlotEncode(word, dir1, dir2);
  } else if (word == "da") {
    std::string cfg_file{}, tgt_dir{};
    std::cin >> word >> tgt_dir >> cfg_file;
    SlotDecode(word, tgt_dir, cfg_file);
  } else if (word == "q") {
    m_run = false;
  } else if (word == "help") {
    SlotHelp();
  } else {
    SlotTip();
  }
}

void Interface::SlotHelp() {
  std::cout
      << "ar _file_  _encoded-file-dir_  _config-dir_\t- to compress a file\n"
      << "da _file_  _decoded-file-dir_  _config-file_\t- to decode a file\n"
      << "help\t- to see this help again\n"
      << "q\t- quit\n";
}

void Interface::SlotTip() {
  if (std::cin.eof())
    m_run = false;
  else
    std::cout << "input not understood, type \"help\" for help\n";
}

void Interface::SlotEncode(const std::string& file_name,
                           const std::string& tgt_dir,
                           const std::string& cfg_dir) {
  Archiver A;
  A.Encode(file_name, tgt_dir, cfg_dir);
  std::cout << "> OK" << std::endl;
}

void Interface::SlotDecode(const std::string& src_file,
                           const std::string& tgt_dir,
                           const std::string& cfg_file) {
  Archiver A;
  A.Decode(src_file, tgt_dir, cfg_file);
  std::cout << "> OK" << std::endl;
}
