#ifndef SRC_HUFFMAN_TREE_H_
#define SRC_HUFFMAN_TREE_H_

#include <map>
#include <queue>
#include <string>

class HuffmanTree {
 public:
  HuffmanTree();
  explicit HuffmanTree(const std::string& src);
  HuffmanTree(const HuffmanTree& other);
  HuffmanTree(HuffmanTree&& other);
  ~HuffmanTree();

  HuffmanTree& operator=(const HuffmanTree& other);
  HuffmanTree& operator=(HuffmanTree&& other);

  void SaveTree(const std::string& file_name);
  void LoadTree(const std::string& file_name);
  std::map<char, std::string> EncodingTree(std::string* tail_code);
  bool DecodeStep(bool step, char* letter);

 private:
  const unsigned kBitsInByte{8};
  struct Node {
    char letter{};
    unsigned count{};
    Node *left{}, *right{};
  };

  Node *m_root{}, *m_tracer{};

  void DeleteTree(Node* root);
  char FindRarest(const std::map<char, unsigned>& src);
  void QueueSmallerFront(std::queue<Node*>* first, std::queue<Node*>* second,
                         Node** target);

  std::string TreeToString();
  void TreeFromString(const std::string& src);
  std::string NodeToString(Node* node, std::string alias);
  void NodeFromString(const std::string& src);

  void EncodingTreeHelper(Node* node, std::map<char, std::string>* res,
                          std::string* tail_code, const std::string& alias);
  void CopyTreeHelper(Node* root, Node* other);
};

#endif  // SRC_HUFFMAN_HUFFMAN_TREE_H_