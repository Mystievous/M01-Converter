//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 9/11/2026
//
#include "M01Core/SongFile.h"

#include "M01Core/ByteReader.h"
#include "M01Core/SaveStructure.h"
#include "M01Core/SongDecoder.h"

#include <optional>
#include <span>
#include <cstddef>
#include <filesystem>

std::optional<SongData> LoadSongFile(std::span<const std::byte> bytes, const std::filesystem::path& path)
{
    ByteReader reader(bytes);
    SongSource source{
        .name = path.stem().string(),
        .songLength = std::nullopt,
    };
    return DecodeSongData(reader, source, 0x00);
}
