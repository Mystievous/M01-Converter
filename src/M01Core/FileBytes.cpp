//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 8/21/2026
//

#include "M01Core/FileBytes.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>

std::vector<std::byte> ReadWholeFile(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Cannot open file: " + path.string());
    }

    const auto size = std::filesystem::file_size(path);
    std::vector<std::byte> bytes(size);

    file.read(reinterpret_cast<char*>(bytes.data()), static_cast<std::streamsize>(size));
    if (!file)
    {
        throw std::runtime_error("Failed to read file: " + path.string());
    }

    return bytes;
}

void WriteWholeFile(const std::filesystem::path& path, std::span<const std::byte> bytes)
{
    std::ofstream file(path, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Cannot open file to write: " + path.string());
    }

    const auto size = bytes.size();

    file.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(size));
    file.close();

    if (!file)
    {
        throw std::runtime_error("Failed to write file: " + path.string());
    }
}
