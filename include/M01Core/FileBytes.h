//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 8/21/2026
//

#ifndef M01_CONVERTER_FILEBYTES_H
#define M01_CONVERTER_FILEBYTES_H

#include <cstddef>
#include <filesystem>
#include <vector>
#include <span>

std::vector<std::byte> ReadWholeFile(const std::filesystem::path& path);

void WriteWholeFile(const std::filesystem::path& path, std::span<const std::byte> bytes);

#endif // M01_CONVERTER_FILEBYTES_H
