#include "archiver.h"

#include <fstream>
#include <iostream>
#include <sstream>

Archiver::~Archiver() { Clean(); }

void Archiver::Encode(const std::string& src_file, const std::string& tgt_dir,
                      const std::string& cfg_dir) {
  std::string content{}, tgt_file{tgt_dir}, cfg_file{cfg_dir};
  ReadFile(src_file, &content);

  Clean();
  m_huffman_tree = new HuffmanTree{content};

  if (tgt_dir.size() > 0 && tgt_dir[tgt_dir.size() - 1] != '/')
    tgt_file.append("/");
  tgt_file.append(GetFile(src_file) + "_encoded");
  std::ofstream out_stream(tgt_file, std::ios::out | std::ios::binary);
  if (!out_stream) throw std::invalid_argument("Encode: cant open target file");
  EncodeToFile(content, &out_stream);

  out_stream.close();
  if (!out_stream.good())
    throw std::invalid_argument("Encode: error during writing");

  if (cfg_dir.size() > 0 && cfg_dir[cfg_dir.size() - 1] != '/')
    cfg_file.append("/");
  cfg_file.append("huffman.cfg");
  m_huffman_tree->SaveTree(cfg_file);
}

void Archiver::Decode(const std::string& src_file, const std::string& tgt_dir,
                      const std::string& config_file) {
  Clean();
  m_huffman_tree = new HuffmanTree;
  m_huffman_tree->LoadTree(config_file);
  std::string content{}, tgt_file{tgt_dir};
  ReadFile(src_file, &content);
  char letter{};

  if (tgt_file[tgt_file.size() - 1] != '/') tgt_file.append("/");
  tgt_file.append(DecodedFileName(GetFile(src_file)));

  std::ofstream stream(tgt_file, std::ios::out | std::ios::binary);
  if (!stream) throw std::invalid_argument("Decode: cant open target file");

  for (auto& each : content) {
    for (unsigned i{kBitsInByte - 1}; i < kBitsInByte; i--) {
      if (m_huffman_tree->DecodeStep(each & (01 << i), &letter)) {
        stream.write(&letter, sizeof(char));
      }
    }
  }
}

void Archiver::Clean() {
  if (m_huffman_tree != nullptr) {
    delete m_huffman_tree;
    m_huffman_tree = nullptr;
  }
}

void Archiver::EncodeToFile(const std::string& src, std::ofstream* output) {
  std::string buffer{};
  std::string tail_code{};
  std::map<char, std::string> enc_tree{
      m_huffman_tree->EncodingTree(&tail_code)};

  for (auto& each : src) {
    buffer.append(enc_tree[each]);
    while (buffer.size() >= kBitsInByte) {
      WriteLetterToFile(&buffer, output);
      buffer = buffer.substr(kBitsInByte);
    }
  }

  if (buffer.size() > 0) {
    while (buffer.size() < kBitsInByte) buffer.append(tail_code);
    WriteLetterToFile(&buffer, output);
  }
}

void Archiver::WriteLetterToFile(std::string* buffer, std::ofstream* stream) {
  char letter{};
  for (unsigned i{}; i < kBitsInByte; i++) {
    letter = (letter << 1) | ((buffer->operator[](i) == '0') ? 00 : 01);
  }
  stream->write(&letter, sizeof(char));
}

void Archiver::ReadFile(const std::string& file_name, std::string* content) {
  std::ifstream stream(file_name, std::ios::in | std::ios::binary);
  if (!stream) {
    throw std::invalid_argument("ReadFile: cant open source file");
  } else {
    std::ostringstream str_stream{};
    str_stream << stream.rdbuf();
    *content = str_stream.str();
  }
  stream.close();
  if (!stream.good())
    throw std::invalid_argument("ReadFile: error during reading");
}

std::string Archiver::DecodedFileName(const std::string& src) {
  std::string res{};
  if (src.size() > 8 && src.substr(src.size() - 8) == "_encoded") {
    res = src.substr(0, src.size() - 8) + "_decoded";
  } else {
    res = src + "_decoded";
  }
  return res;
}

std::string Archiver::GetFile(const std::string& src) {
  std::string res{};
  size_t pos{src.find_last_of('/')};
  if (pos < src.size())
    res = src.substr(pos);
  else
    res = src;
  return res;
}
