//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 9/12/2026
//

#ifndef SONGENCODER_H
#define SONGENCODER_H

#include "M01Core/SaveStructure.h"

#include <vector>
#include <cstddef>

std::vector<std::byte> EncodeSongData(const SongData& song, const SaveFormat saveFormat);

#endif // !SONGENCODER_H
