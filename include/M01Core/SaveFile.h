//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/21/2025
//

#ifndef SAVEFILE_H
#define SAVEFILE_H

#include "M01Core/SaveStructure.h"

class SaveFile
{
    bool isValid = false;
    std::vector<SongData> songs;

public:
    explicit SaveFile(const std::string path);

    [[nodiscard]] bool IsValid() const;

    [[nodiscard]] int GetNumberOfSongs() const { return static_cast<int>(songs.size()); }

    [[nodiscard]] const std::vector<SongData>& GetSongs() const { return songs; }
};

#endif // SAVEFILE_H
