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
      attribute.compression_type = CompressionType::SQ;
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

}  // namespace ply_processor