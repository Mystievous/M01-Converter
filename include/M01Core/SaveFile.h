//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/21/2025
//

#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <span>
#include <cstddef>

#include "Song.h"

class SaveFile
{
    bool isValid = false;
    std::vector<Song> songs;

public:
    explicit SaveFile(std::span<const std::byte> data);

    [[nodiscard]] bool IsValid() const;

    [[nodiscard]] int GetNumberOfSongs() const
    {
        return static_cast<int>(songs.size());
    }

    [[nodiscard]] const std::vector<Song>& GetSongs() const
    {
        return songs;
    }
};

#endif // SAVEFILE_H
