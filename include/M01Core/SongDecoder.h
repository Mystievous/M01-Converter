//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 8/27/2026
//

#ifndef M01_CONVERTER_SONGDECODER_H
#define M01_CONVERTER_SONGDECODER_H

#include "M01Core/ByteReader.h"
#include "M01Core/SaveStructure.h"

void DecodeSongData(ByteReader& reader, const SongIdentifier& identifier);

#endif //M01_CONVERTER_SONGDECODER_H
