//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/21/2025
//

#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <filesystem>
#include <cstddef>
#include <span>
#include <string_view>

#include "M01Core/SaveStructure.h"

constexpr std::string_view kFileSignature = "M01W";

class SaveFile
{
    bool isValid = false;
    std::vector<SongData> songs;

public:
    explicit SaveFile(std::span<const std::byte> bytes, const std::filesystem::path& parentDir);

    [[nodiscard]] bool IsValid() const;

    [[nodiscard]] int GetNumberOfSongs() const { return static_cast<int>(songs.size()); }

    [[nodiscard]] const std::vector<SongData>& GetSongs() const { return songs; }
};

#endif // SAVEFILE_H
