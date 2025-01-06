#include "data.h"

namespace ply_processor {
bool DataLoader::LoadConfig(const std::string& filename) {
  YAML::Node config = YAML::LoadFile(filename);

  if (!config["attributes"]) {
    std::cout << "Error config format.\n";
    return false;
  }
  const YAML::Node& attributes = config["attributes"];
  for (const auto& attr : attributes) {
    Attribute attribute;

    attribute.name = attr["name"].as<std::string>();
    attribute.type = attr["type"].as<std::string>();

    // compression type
    if (attr["compression_type"]) {
      attribute.compression_type =
          attr["compression_type"].as<CompressionType>();
    } else {
      attribute.compression_type = CompressionType::NONE;
    }

    // size of current attribute
    if (attr["components"]) {
      attribute.components = attr["components"].as<int>();
    } else {
      attribute.components = 1;
    }

    name_to_attributes_[attribute.name] = attribute;
  }
  return true;
}

BaseData::BaseData(
    const std::unordered_map<std::string, Attribute>& name_to_attributes) {
  // Init vectors for each name
  for (const auto& name_to_attribute : name_to_attributes) {
    if (name_to_attribute.second.type == "int") {
      int_variable_stores_[name_to_attribute.second.name] = std::vector<int>();
    } else if (name_to_attribute.second.type == "float") {
      float_variable_stores_[name_to_attribute.second.name] =
          std::vector<float>();
    } else if (name_to_attribute.second.type == "uchar") {
      uchar_variable_stores_[name_to_attribute.second.name] =
          std::vector<unsigned char>();
    }
  }
}

}  // namespace ply_processor