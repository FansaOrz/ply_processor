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
bool VectorQuantization(std::shared_ptr<DataLoader> data_loader,
                        const Attribute& attribute,
                        std::vector<std::vector<float>>& codebooks,
                        std::vector<size_t>& indices);

bool ScalarQuantization(std::shared_ptr<DataLoader> data_loader,
                        const Attribute& attribute,
                        const std::vector<uint8_t>& input, float& min_val,
                        float& max_val);

bool HuffmanCoding(std::shared_ptr<DataLoader> data_loader,
                   const Attribute& attribute,
                   std::unordered_map<char, std::string>& char_to_code,
                   std::vector<uint8_t>& encoded_data, int& padding_bits);

}  // namespace ply_processor
