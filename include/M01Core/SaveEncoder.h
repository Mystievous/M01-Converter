//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 9/12/2026
//

#ifndef SAVEENCODER_H
#define SAVEENCODER_H

#include "M01Core/SaveStructure.h"

#include <span>
#include <vector>
#include <string>

struct EncodedSaveFile
{
    std::string filename;
    std::vector<std::byte> bytes;
};

std::vector<std::byte> EncodeSaveIndex(std::span<const SongIdentifier> songs, const SaveFormat saveFormat);

std::vector<EncodedSaveFile> EncodeSave(std::span<const std::optional<SongData>> songs, const SaveFormat saveFormat);

#endif // !SAVEENCODER_H
