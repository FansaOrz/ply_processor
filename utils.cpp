

#include "utils.h"

namespace ply_processor {

// 解析 PLY 文件
void LoadPLY(const std::string& filename,
             std::shared_ptr<DataLoader> data_loader) {
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + filename);
  }

  // 读取头部
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

  // PLY数据解析到DataLoader中
  for (int i = 0; i < vertex_count; ++i) {
    BaseData* new_ply_data = data_loader->NewData();
    for (const auto& name : data_loader->GetAttributeNames()) {
      const int count = name_to_attributes.at(name).components;

      if (name_to_attributes.at(name).type == "float") {
        file.read(reinterpret_cast<char*>(
                      &new_ply_data->float_variable_stores_[name]),
                  sizeof(float) * count);
      } else if (name_to_attributes.at(name).type == "int") {
        file.read(
            reinterpret_cast<char*>(&new_ply_data->int_variable_stores_[name]),
            sizeof(int) * count);
      } else if (name_to_attributes.at(name).type == "uchar") {
        file.read(reinterpret_cast<char*>(
                      &new_ply_data->uchar_variable_stores_[name]),
                  sizeof(u_char) * count);
      }
    }
  }
}

}  // namespace ply_processor