#include "decompressor.h"

#include "utils.h"

namespace ply_processor {
DeCompressor::DeCompressor(const std::string& compressed_file_path) {
  compressed_file_path_ = compressed_file_path;
}

/*
 * @brief Compress the loaded data.
 */
bool DeCompressor::DeCompress() {
  // TODO: Implement the decompression logic.
  // Note: Just same as the Compressor::Compress() function.
  return true;
}

bool DeCompressor::LoadBinFormatFile() {
  // TODO: Implement the decompression logic.
  // Note: Just same as the Compressor::OutputCompressedData() function.
  // Load bin into
  // std::map<std::string, VectorQuantizationData>
  // name_to_vector_quantization_results_; std::map<std::string,
  // ScalarQuantizationData> name_to_scalar_quantization_results_;
  // std::map<std::string, HuffmanCodingData> name_to_huffman_coding_results_;

  return true;
}

}  // namespace ply_processor