#include "huffman_tree.h"

#include <fstream>
#include <iostream>
#include <sstream>

HuffmanTree::HuffmanTree() { m_root = new Node{0, 0, nullptr, nullptr}; }

HuffmanTree::HuffmanTree(const std::string& src) {
  std::queue<Node*> first_q{}, second_q{};
  std::map<char, unsigned> counter;
  for (auto& each : src) {
    counter[each]++;
  }

  first_q.push(new Node{0, 0, nullptr, nullptr});
  while (counter.size() > 0) {
    char key{FindRarest(counter)};
    first_q.push(new Node{key, counter[key], nullptr, nullptr});
    counter.erase(key);
  }

  while (first_q.size() + second_q.size() > 1) {
    Node *left, *right;
    QueueSmallerFront(&first_q, &second_q, &right);
    QueueSmallerFront(&first_q, &second_q, &left);
    second_q.push(new Node{0, left->count + right->count, left, right});
  }

  m_root = second_q.front();
}

HuffmanTree::HuffmanTree(const HuffmanTree& other) : HuffmanTree() {
  *this = other;
}

HuffmanTree::HuffmanTree(HuffmanTree&& other) : HuffmanTree() {
  *this = std::move(other);
}

HuffmanTree::~HuffmanTree() { DeleteTree(m_root); }

HuffmanTree& HuffmanTree::operator=(const HuffmanTree& other) {
  if (this != &other) {
    DeleteTree(m_root);
    m_root = new Node{0, 0, nullptr, nullptr};
    CopyTreeHelper(m_root, other.m_root);
    m_tracer = m_root;
  }
  return *this;
}

HuffmanTree& HuffmanTree::operator=(HuffmanTree&& other) {
  if (this != &other) {
    std::swap(m_root, other.m_root);
    std::swap(m_tracer, other.m_tracer);
  }
  return *this;
}

void HuffmanTree::SaveTree(const std::string& file_name) {
  std::ofstream stream(file_name, std::ios::out | std::ios::binary);
  if (!stream) throw std::invalid_argument("SaveTree: cant open file");

  std::string voc = TreeToString();
  stream.write(voc.c_str(), voc.size() * sizeof(char));

  stream.close();
  if (!stream.good())
    throw std::invalid_argument("SaveTree: error while writing");
}

void HuffmanTree::LoadTree(const std::string& file_name) {
  std::ifstream stream(file_name, std::ios::in | std::ios::binary);
  std::string voc{};
  if (!stream) {
    throw std::invalid_argument("LoadTree: cant open file");
  } else {
    DeleteTree(m_root);
    m_root = new Node{0, 0, nullptr, nullptr};
    std::ostringstream str_stream{};
    str_stream << stream.rdbuf();
    voc = str_stream.str();
  }

  TreeFromString(voc);

  stream.close();
  if (!stream.good())
    throw std::invalid_argument("LoadTree: error while reading");

  m_tracer = m_root;
}

std::map<char, std::string> HuffmanTree::EncodingTree(std::string* tail_code) {
  std::map<char, std::string> res{};
  EncodingTreeHelper(m_root, &res, tail_code, "");
  return res;
}

bool HuffmanTree::DecodeStep(bool step, char* letter) {
  bool res{false};
  m_tracer = (step) ? m_tracer->right : m_tracer->left;
  if (m_tracer->left == nullptr) {
    if (m_tracer->count > 0) res = true;
    *letter = m_tracer->letter;
    m_tracer = m_root;
  }
  return res;
}

void HuffmanTree::DeleteTree(Node* root) {
  if (root != nullptr) {
    DeleteTree(root->left);
    DeleteTree(root->right);
    delete root;
  }
}

char HuffmanTree::FindRarest(const std::map<char, unsigned>& src) {
  unsigned min{};
  char key{};
  for (auto& each : src) {
    if (min == 0 || each.second < min) {
      key = each.first;
      min = each.second;
    }
  }
  return key;
}

void HuffmanTree::QueueSmallerFront(std::queue<Node*>* first,
                                    std::queue<Node*>* second, Node** target) {
  if (first->size() == 0) {
    *target = second->front();
    second->pop();
  } else if (second->size() == 0) {
    *target = first->front();
    first->pop();
  } else if (first->front()->count < second->front()->count) {
    *target = first->front();
    first->pop();
  } else {
    *target = second->front();
    second->pop();
  }
}

std::string HuffmanTree::TreeToString() { return NodeToString(m_root, ""); }

void HuffmanTree::TreeFromString(const std::string& src) {
  unsigned pos{};
  while (pos < src.size()) {
    unsigned num{(src[pos + 1] - 1) / kBitsInByte + 3};
    NodeFromString(src.substr(pos, num));
    pos += num;
  }
}

std::string HuffmanTree::NodeToString(Node* node, std::string alias) {
  if (node == nullptr) throw std::logic_error("NodeToString: empty tree");
  std::string res{};
  if (node->left == nullptr) {
    if (node->count > 0) {
      res = node->letter;
      res += (char)alias.size();
      char letter{};
      unsigned num{};
      for (auto& each : alias) {
        num++;
        if (each == '1') letter = letter | (01 << (kBitsInByte - num));
        if (num == kBitsInByte) {
          res += letter;
          num = 0;
          letter = 0;
        }
      }
      if (num > 0) res += letter;
    }
  } else {
    res = NodeToString(node->left, alias + "0") +
          NodeToString(node->right, alias + "1");
  }
  return res;
}

void HuffmanTree::NodeFromString(const std::string& src) {
  char letter = src[0];
  unsigned num = src[1];
  Node* node{m_root};

  for (unsigned i{}; i < num; i++) {
    bool turn = (src[2 + i / kBitsInByte] &
                 (01 << (kBitsInByte - (i % kBitsInByte) - 1)));
    if (node->left == nullptr) {
      node->left = new Node{0, 0, nullptr, nullptr};
      node->right = new Node{0, 0, nullptr, nullptr};
    }
    if (turn)
      node = node->right;
    else
      node = node->left;
  }

  node->letter = letter;
  node->count = 1;
}

void HuffmanTree::EncodingTreeHelper(Node* node,
                                     std::map<char, std::string>* res,
                                     std::string* tail_code,
                                     const std::string& alias) {
  if (node == nullptr)
    throw std::logic_error("EncodingTreeHelper: got null node");
  if (node->left == nullptr) {
    if (node->count == 0) {
      *tail_code = alias;
    } else {
      res->operator[](node->letter) = alias;
    }
  } else {
    EncodingTreeHelper(node->left, res, tail_code, alias + "0");
    EncodingTreeHelper(node->right, res, tail_code, alias + "1");
  }
}

void HuffmanTree::CopyTreeHelper(Node* root, Node* other) {
  root->count = other->count;
  root->letter = other->letter;
  if (other->left != nullptr) {
    root->left = new Node{0, 0, nullptr, nullptr};
    root->right = new Node{0, 0, nullptr, nullptr};
    CopyTreeHelper(root->left, other->left);
    CopyTreeHelper(root->right, other->right);
  }
}
