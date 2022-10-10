#ifndef SRC_INTERFACE_H_
#define SRC_INTERFACE_H_

#include "archiver.h"

class Interface {
 public:
  Interface();
  ~Interface();

  void Show();

 private:
  bool m_run{true};

  void ScanInput();
  void SlotHelp();
  void SlotTip();
  void SlotEncode(const std::string& file_name, const std::string& tgt_dir,
                  const std::string& cfg_dir);
  void SlotDecode(const std::string& src_file, const std::string& tgt_dir,
                  const std::string& cfg_file);
  void Clean();
};

#endif  // SRC_HUFFMAN_INTERFACE_H_
