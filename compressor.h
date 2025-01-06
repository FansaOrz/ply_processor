#pragma once

#include <iostream>
#include "data.h"

namespace ply_processor {
class Compressor {
 public:
  Compressor(const std::string& config_path, const std::string& ply_path);
  ~Compressor() = default;

  bool Compress();
  bool OutputCompressedData(const std::string& output_path);

 private:
  std::shared_ptr<DataLoader> data_loader_;

  // 每一个属性的向量量化后对应的codebook
  std::map<std::string, std::vector<std::vector<float>>> name_to_codebooks_;
};
}  // namespace ply_processor