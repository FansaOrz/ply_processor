

#include "utils.h"

namespace ply_processor {

void LoadPLY(const std::string& filename,
             std::shared_ptr<DataLoader> data_loader) {
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + filename);
  }

  // Load header information
  std::string line;
  int vertex_count = 0;
  while (std::getline(file, line)) {
    if (line.substr(0, 14) == "element vertex") {
      std::istringstream iss(line);
      std::string element, vertex;
      iss >> element >> vertex >> vertex_count;
    }
    if (line == "end_header") {
      break;
    }
  }

  const auto& name_to_attributes = data_loader->GetAttributes();

  // Load vertex data
  for (int i = 0; i < vertex_count; ++i) {
    BaseData* new_ply_data = data_loader->NewData();
    for (const auto& name : data_loader->GetAttributeNames()) {
      const int count = name_to_attributes.at(name).components;

      if (name_to_attributes.at(name).type == "float") {
        std::vector<float> temp_floats(count);
        file.read(reinterpret_cast<char*>(&temp_floats), sizeof(float) * count);
        new_ply_data->float_variable_stores_[name] = temp_floats;
      } else if (name_to_attributes.at(name).type == "int") {
        std::vector<int> temp_ints(count);

        file.read(reinterpret_cast<char*>(&temp_ints), sizeof(int) * count);
        new_ply_data->int_variable_stores_[name] = temp_ints;

      } else if (name_to_attributes.at(name).type == "uchar") {
        std::vector<u_char> temp_uchars(count);

        file.read(reinterpret_cast<char*>(&temp_uchars),
                  sizeof(u_char) * count);
        new_ply_data->uchar_variable_stores_[name] = temp_uchars;
      }
    }
  }
}

bool VectorQuantization(const std::vector<std::vector<float>>& input_vectors,
                        std::vector<std::vector<float>>& codebooks,
                        std::vector<size_t>& indices) {
  // TODO: implement vector quantization
  return true;
}

template <typename T>
std::pair<std::vector<uint8_t>, std::pair<T, T>> ScalarQuantization(
    const std::vector<T>& input) {
  // Get min and max values from the input vector
  T min_val = *std::min_element(input.begin(), input.end());
  T max_val = *std::max_element(input.begin(), input.end());

  // If all values are the same, return a vector of 255s and min/max as
  if (min_val == max_val) {
    return {std::vector<uint8_t>(input.size(), 255), {min_val, max_val}};
  }

  std::vector<uint8_t> quantized;
  const float scale = 255.0f / static_cast<float>(max_val - min_val);

  for (const T& value : input) {
    const T quantized_value = std::min(std::max(value, min_val), max_val);
    quantized.push_back(
        static_cast<uint8_t>(std::lround((quantized_value - min_val) * scale)));
  }

  return {quantized, {min_val, max_val}};
}

}  // namespace ply_processor