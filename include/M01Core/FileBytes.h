//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 8/21/2026
//

#ifndef M01_CONVERTER_FILEBYTES_H
#define M01_CONVERTER_FILEBYTES_H

#include <cstddef>
#include <string>
#include <vector>

std::vector<std::byte> ReadWholeFile(const std::string& path);

#endif //M01_CONVERTER_FILEBYTES_H
