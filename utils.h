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
/*
 * @brief Loads a PLY file into a DataLoader object.
 * 
 * @param filename The path to the PLY file.
 * @param data_loader A shared pointer to a DataLoader object.
 */
void LoadPLY(const std::string& filename,
             std::shared_ptr<DataLoader> data_loader);


/*
 * @brief Performs vector quantization on a set of input vectors.
 * 
 * @param input_vectors A vector of vectors representing the input data.
 * @param codebooks A vector of vectors representing the output codebooks.
 * @param indices A vector of size_t representing the index of each input 
 */
bool VectorQuantization(const std::vector<std::vector<float>>& input_vectors,
                        std::vector<std::vector<float>>& codebooks,
                        std::vector<size_t>& indices);


std::vector<uint8_t> ScalarQuantization(const std::vector<float>& input, float& min_val, float& max_val);

}  // namespace ply_processor
