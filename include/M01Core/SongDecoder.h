//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 8/27/2026
//

#ifndef M01_CONVERTER_SONGDECODER_H
#define M01_CONVERTER_SONGDECODER_H

#include "M01Core/ByteReader.h"
#include "M01Core/SaveStructure.h"
#include <cstdint>

std::optional<SongData> DecodeSongData(ByteReader& reader, const SongIdentifier& identifier,
                                       const uint32_t startAddress);

#endif // M01_CONVERTER_SONGDECODER_H
