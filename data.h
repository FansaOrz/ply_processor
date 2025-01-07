#pragma once

#include <yaml-cpp/yaml.h>

#include <iostream>
#include <vector>

namespace ply_processor {

// Type of compression
enum CompressionType {
  VQ = 0,  // Vector Quantization
  SQ,      // Scalar Quantization
  HC,      // Huffman Coding

};

struct Attribute {
  std::string name;
  CompressionType compression_type;
  std::string type;  // "int", "float", "uchar"
  int components = 0;
};

struct ScalarQuantizationData {
  std::vector<uint8_t> data;
  float min_value;
  float max_value;
};

struct VectorQuantizationData {
  std::vector<std::vector<float>> code_book;
  std::vector<size_t> indices;
};

struct HuffmanCodingData {
  std::unordered_map<char, std::string> char_to_code;
  std::vector<uint8_t> encoded_data;

  // padding bits of last byte
  int padding_bits;
};

/*
 * Each basedata save one vertex of ply file.
 */
class BaseData {
 public:
  BaseData() = default;
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
    data_.emplace_back(new BaseData());
    return data_.back();
  }

 private:
  // We need to know the order of config, so we use unordered_map and vector.
  std::unordered_map<std::string, Attribute> name_to_attributes_;
  std::vector<std::string> name_order_;

  std::vector<BaseData*> data_;
};

}  // namespace ply_processor