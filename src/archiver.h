#ifndef SRC_HUFFMAN_ARCHIVER_H_
#define SRC_HUFFMAN_ARCHIVER_H_

#include "huffman_tree.h"

class Archiver {
 public:
  Archiver() {}
  ~Archiver();

  void Encode(const std::string& src_file, const std::string& tgt_dir,
              const std::string& cfg_dir);
  void Decode(const std::string& src_file, const std::string& tgt_dir,
              const std::string& cfg_file);

 private:
  const unsigned kBitsInByte{8};

  HuffmanTree* m_huffman_tree{};

  void Clean();
  void EncodeToFile(const std::string& src, std::ofstream* output);
  void WriteLetterToFile(std::string* buffer, std::ofstream* stream);
  void ReadFile(const std::string& file_name, std::string* content);
  std::string DecodedFileName(const std::string& src);
  std::string GetFile(const std::string& src);
};

#endif  // SRC_HUFFMAN_ARCHIVER_H_
