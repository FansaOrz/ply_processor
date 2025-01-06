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
  // Process each attribute in the data_loader_.
  for (const auto& name_to_attribute : name_to_attributes) {
    // Vector quantization
    if (name_to_attribute.second.compression_type == CompressionType::VQ) {
      if (!VectorQuantization())
        name_to_codebooks_[name_to_attributes.first] = {codebook, index_vector};
    } 
    // Scalar quantization
    else if (name_to_attributes.second.compression_type ==
               CompressionType::SQ) {
      if (name_to_attributes.second != 1) {
        std::cerr << "Scalar quantization only supports 1-D data.\n";
        return false;
      }
      // Get data according to the name
      auto data =
          data_loader_->GetDataAccordingName(name_to_attributes.first, );
              name_to_scalar_quantization_results_[name_to_attributes.first] =
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