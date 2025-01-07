

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

bool VectorQuantization(std::shared_ptr<DataLoader> data_loader,
                        const Attribute& attribute,
                        std::vector<std::vector<float>>& codebooks,
                        std::vector<size_t>& indices) {
  // TODO: implement vector quantization
  return true;
}

bool ScalarQuantization(std::shared_ptr<DataLoader> data_loader,
                        const Attribute& attribute,
                        const std::vector<uint8_t>& input, float& min_val,
                        float& max_val) {
  // TODO: implement vector quantization
  return true;
}

bool HuffmanCoding(std::shared_ptr<DataLoader> data_loader,
                   const Attribute& attribute,
                   std::unordered_map<char, std::string>& char_to_code,
                   std::vector<uint8_t>& encoded_data, int& padding_bits) {
  // TODO: implement vector quantization
  return true;
}

}  // namespace ply_processor