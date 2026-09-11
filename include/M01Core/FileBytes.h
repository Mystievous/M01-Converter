//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 8/21/2026
//

#ifndef M01_CONVERTER_FILEBYTES_H
#define M01_CONVERTER_FILEBYTES_H

#include <cstddef>
#include <filesystem>
#include <vector>

std::vector<std::byte> ReadWholeFile(const std::filesystem::path& path);

#endif // M01_CONVERTER_FILEBYTES_H
