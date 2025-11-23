//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/21/2025
//

#include "M01Core/SaveFile.h"

#include <string>
#include <string_view>

constexpr std::string_view kFileSignature = "M01W";

SaveFile::SaveFile(FILE *saveFile)
{
    FileHeader header{};
    fread(&header, sizeof(FileHeader), 1, saveFile);

    isValid = std::string(header.signature, 4) == kFileSignature;

    if (isValid)
    {
        for (const auto &songIdentifier : header.songIdentifiers)
        {
            if (songIdentifier.songHasData)
            {
                songs.emplace_back(saveFile, songIdentifier);
            }
        }
    }
}

bool SaveFile::IsValid() const
{
    return isValid;
}
