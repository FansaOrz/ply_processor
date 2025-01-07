#pragma once

#include <iostream>
#include "data.h"

namespace ply_processor {
class DeCompressor {
 public:
  DeCompressor(const std::string& compressed_file_path);
  ~DeCompressor() = default;

  bool DeCompress();
  bool LoadBinFormatFile();

 private:
  std::string compressed_file_path_;

  std::shared_ptr<DataLoader> data_loader_;

  std::map<std::string, VectorQuantizationData>
      name_to_vector_quantization_results_;
  std::map<std::string, ScalarQuantizationData>
      name_to_scalar_quantization_results_;
  std::map<std::string, HuffmanCodingData> name_to_huffman_coding_results_;
};
}  // namespace ply_processor