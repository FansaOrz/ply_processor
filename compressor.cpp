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
      VectorQuantizationData vq_data;
      if (!VectorQuantization(data_loader_, name_to_attribute.second,
                              vq_data.code_book, vq_data.indices)) {
        std::cerr << "Vector quantization failed.\n";
        return false;
      }
      // Store the codebook and index vector for each attribute.
      name_to_vector_quantization_results_[name_to_attribute.first] = vq_data;

    }

    // Scalar quantization
    else if (name_to_attribute.second.compression_type == CompressionType::SQ) {
      ScalarQuantizationData scalar_data;
      if (!ScalarQuantization(data_loader_, name_to_attribute.second,
                              scalar_data.data, scalar_data.min_value,
                              scalar_data.max_value)) {
        std::cerr << "Scalar quantization failed.\n";
        return false;
      }
      name_to_scalar_quantization_results_[name_to_attribute.first] =
          scalar_data;
    }

    // Huffman Coding
    else if (name_to_attribute.second.compression_type == CompressionType::HC) {
      HuffmanCodingData huffman_data;
      if (!HuffmanCoding(data_loader_, name_to_attribute.second,
                         huffman_data.char_to_code, huffman_data.encoded_data,
                         huffman_data.padding_bits)) {
        std::cerr << "Huffman coding failed.\n";
        return false;
      }
      name_to_huffman_coding_results_[name_to_attribute.first] = huffman_data;
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
  // Header generation
  // Record each attribute's compression type
  std::vector<std::pair<std::string, std::string>> header_data;
  for (const auto& name_to_vector_quantization_result :
       name_to_vector_quantization_results_) {
    header_data.push_back(
        std::make_pair(name_to_vector_quantization_result.first, "VQ"));
  }
  for (const auto& name_to_scalar_quantization_result :
       name_to_scalar_quantization_results_) {
    header_data.push_back(
        std::make_pair(name_to_scalar_quantization_result.first, "SQ"));
  }
  for (const auto& name_to_huffman_coding : name_to_huffman_coding_results_) {
    header_data.push_back(std::make_pair(name_to_huffman_coding.first, "HC"));
  }

  std::ofstream output_file(output_path, std::ios::binary);
  if (!output_file) {
    throw std::runtime_error("Failed to open file for writing.");
  }

  // Write the header to the output file
  size_t header_size = header_data.size();
  output_file.write(reinterpret_cast<const char*>(&header_size),
                    sizeof(size_t));
  for (const auto& entry : header_data) {
    size_t name_len = entry.first.size();
    size_t type_len = entry.second.size();

    // Write the name length and name
    output_file.write(reinterpret_cast<const char*>(&name_len), sizeof(size_t));
    output_file.write(entry.first.data(), name_len);

    // Write the type length and type
    output_file.write(reinterpret_cast<const char*>(&type_len), sizeof(size_t));
    output_file.write(entry.second.data(), type_len);
  }

  // Write the compressed data to the output file
  for (const auto& header : header_data) {
    if (header.second == "VQ") {
      const auto& vq_data = name_to_vector_quantization_results_[header.first];
      // Write 2-D codebook
      size_t num_vq_data = vq_data.code_book.size();
      output_file.write(reinterpret_cast<const char*>(&num_vq_data),
                        sizeof(size_t));
      for (const auto& vec : vq_data.code_book) {
        size_t vec_size = vec.size();
        output_file.write(reinterpret_cast<const char*>(&vec_size),
                          sizeof(size_t));
        output_file.write(reinterpret_cast<const char*>(vec.data()),
                          vec_size * sizeof(float));
      }
      // Write 1-D index vector
      size_t indices_size = vq_data.indices.size();
      output_file.write(reinterpret_cast<const char*>(&indices_size),
                        sizeof(size_t));
      output_file.write(reinterpret_cast<const char*>(vq_data.indices.data()),
                        indices_size * sizeof(size_t));
    } else if (header.second == "SQ") {
      const auto& sq_data = name_to_scalar_quantization_results_[header.first];
      // Write 1-D data
      size_t num_data = sq_data.data.size();
      output_file.write(reinterpret_cast<const char*>(&num_data),
                        sizeof(size_t));
      output_file.write(reinterpret_cast<const char*>(sq_data.data.data()),
                        num_data * sizeof(uint8_t));
      // Write sigle value
      output_file.write(reinterpret_cast<const char*>(&sq_data.min_value),
                        sizeof(float));
      output_file.write(reinterpret_cast<const char*>(&sq_data.max_value),
                        sizeof(float));
    } else if (header.second == "HC") {
      const auto& hc_data = name_to_huffman_coding_results_[header.first];
      // Write char-to-code map
      size_t map_size = hc_data.char_to_code.size();
      output_file.write(reinterpret_cast<const char*>(&map_size),
                        sizeof(size_t));

      // write each key-value pair
      for (const auto& pair : hc_data.char_to_code) {
        output_file.write(reinterpret_cast<const char*>(&pair.first),
                          sizeof(char));

        size_t value_size = pair.second.size();
        output_file.write(reinterpret_cast<const char*>(&value_size),
                          sizeof(size_t));
        output_file.write(pair.second.data(), value_size);
      }
      // Write 1-D encoded_data
      size_t num_data = hc_data.encoded_data.size();
      output_file.write(reinterpret_cast<const char*>(&num_data),
                        sizeof(size_t));
      output_file.write(
          reinterpret_cast<const char*>(hc_data.encoded_data.data()),
          num_data * sizeof(uint8_t));
      // Write padding_bits
      output_file.write(reinterpret_cast<const char*>(&hc_data.padding_bits),
                        sizeof(int));
    }
  }

  output_file.close();

  return true;
}

}  // namespace ply_processor