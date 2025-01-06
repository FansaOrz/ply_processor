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

template <typename T>
std::vector<T> DataLoader::GetDataAccordingName(const std::string& name) {
  if (T == float) {
    std::vector<float> data;
    for (const auto& data_base : data_) {
      if (data->float_variable_stores_.find(name) !=
          data->float_variable_stores_.end()) {
        data.push_back(data->float_variable_stores_[name][0]);
      }
    }
    return data;
  }
  if (T == int) {
    std::vector<int> data;
    for (const auto& data_base : data_) {
      if (data->int_variable_stores_.find(name) !=
          data->int_variable_stores_.end()) {
        data.push_back(data->int_variable_stores_[name][0]);
      }
    }
    return data;
  }
  if (T == uchar) {
    std::vector<uchar> data;
    for (const auto& data_base : data_) {
      if (data->uchar_variable_stores_.find(name) !=
          data->uchar_variable_stores_.end()) {
        data.push_back(data->uchar_variable_stores_[name][0]);
      }
    }
    return data;
  }
  return {};
}

}  // namespace ply_processor