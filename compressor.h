#pragma once

#include <iostream>
#include "data.h"

namespace ply_processor {
class Compressor {
 public:
  Compressor(const std::string& config_path, const std::string& ply_path);
  ~Compressor() = default;

  /*
   * @brief Compress the loaded data.
   */
  bool Compress();
  /*
   * @brief Output the compressed data to a binary file.
   *
   * @param output_path The path of the output file.
   */
  bool OutputCompressedData(const std::string& output_path);

 private:
  std::shared_ptr<DataLoader> data_loader_;

  std::map<std::string, VectorQuantizationData>
      name_to_vector_quantization_results_;
  std::map<std::string, ScalarQuantizationData>
      name_to_scalar_quantization_results_;
  std::map<std::string, HuffmanCodingData> name_to_huffman_coding_results_;
};
}  // namespace ply_processor