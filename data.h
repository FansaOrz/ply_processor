#pragma once

#include <yaml-cpp/yaml.h>
#include <iostream>
#include <vector>

namespace ply_processor {

// Type of compression
enum CompressionType {
  NONE = 0,
  VQ,  // Vector Quantization
  SQ,  // Scale Quantization
  HC,  // Huffman Coding

};

struct Attribute {
  std::string name;
  CompressionType compression_type;
  std::string type;  // "int", "float", "uchar"
  int components = 0;
};

/*
 * 通用的保存ply原始数据
 */
class BaseData {
 public:
  BaseData(
      const std::unordered_map<std::string, Attribute>& name_to_attributes);
  ~BaseData() = default;

 public:
  // e.g. "xyz" -> "std::vector<float>"
  std::map<std::string, std::vector<float>> float_variable_stores_;
  // e.g. "anything" -> "std::vector<int>"
  std::map<std::string, std::vector<int>> int_variable_stores_;
  // e.g. "rgb" -> "std::vector<u_char>"
  std::map<std::string, std::vector<u_char>> uchar_variable_stores_;
};

class DataLoader {
 public:
  DataLoader(const std::string& filename) { LoadConfig(filename); }
  ~DataLoader() = default;

  bool LoadConfig(const std::string& filename);

  std::vector<std::string> GetAttributeNames() const { return name_order_; }

  std::unordered_map<std::string, Attribute> GetAttributes() const {
    return name_to_attributes_;
  }

  BaseData* GetData(int index) { return data_[index]; }

  BaseData* NewData() {
    data_.emplace_back(new BaseData(GetAttributes()));
    return data_.back();
  }

 private:
  // 需要按照config的顺序排序
  std::unordered_map<std::string, Attribute> name_to_attributes_;
  std::vector<std::string> name_order_;

  std::vector<BaseData*> data_;
};

}  // namespace ply_processor