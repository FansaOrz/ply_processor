#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "data.h"

namespace ply_processor {
void LoadPLY(const std::string& filename,
             std::shared_ptr<DataLoader> data_loader);
}  // namespace ply_processor
