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

  // codebook of each attribute after vector quantization
  std::map<std::string, std::vector<std::vector<float>>> name_to_codebooks_;
  std::map<std::string, ScalarQuantizationData> name_to_scalar_quantization_results_;
};
}  // namespace ply_processor