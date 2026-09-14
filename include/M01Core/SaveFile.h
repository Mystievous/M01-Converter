//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/21/2025
//

#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <filesystem>
#include <cstddef>
#include <span>
#include <algorithm>
#include <optional>

#include "M01Core/SaveStructure.h"

class SaveFile
{
    bool isValid = false;
    std::vector<std::optional<SongData>> songs;

public:
    explicit SaveFile(std::span<const std::byte> bytes, const std::filesystem::path& parentDir);

    [[nodiscard]] bool IsValid() const;

    [[nodiscard]] int GetIndexSize() const { return static_cast<int>(songs.size()); }

    [[nodiscard]] int GetNumberOfSongsWithData() const
    {
        return std::count_if(songs.begin(), songs.end(),
                             [](const std::optional<SongData>& song) { return song.has_value(); });
    }

    [[nodiscard]] const std::vector<std::optional<SongData>>& GetSongs() const { return songs; }
};

#endif // SAVEFILE_H
