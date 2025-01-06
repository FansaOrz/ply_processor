#include "compressor.h"
#include "utils.h"

namespace ply_processor {
Compressor::Compressor(const std::string& config_path,
                       const std::string& ply_path) {
  data_loader_ = std::make_shared<DataLoader>();
  data_loader_->LoadConfig(config_path);

  // Load ply into data_
  LoadPLY(ply_path, data_loader_);
}

/*
 * @brief Compress the loaded data.
 */
bool Compressor::Compress() {
  const auto& name_to_attributes = data_loader_->GetAttributes();
  for (const auto& name_to_attribute : name_to_attributes) {
    // 对于要做向量量化的属性，调用向量量化的函数，得到压缩后的codebook和index vector
    if (name_to_attribute.second.compression_type == CompressionType::VQ) {
        // 
    }
  }

  return true;
}

/*
 * @brief Output the compressed data to a binary file.
 *
 * @param output_path The path of the output file.
 */
bool Compressor::OutputCompressedData(const std::string& output_path) {
  return true;
}

}  // namespace ply_processor