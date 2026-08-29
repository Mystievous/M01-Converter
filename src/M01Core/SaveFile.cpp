//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/21/2025
//

#include "M01Core/SaveFile.h"

#include <string>
#include <string_view>
#include <format>
#include <iostream>
#include <algorithm>
#include <cstdint>
#include <vector>

#include "M01Core/SongDecoder.h"
#include "M01Core/ByteReader.h"

constexpr std::string_view kFileSignature = "M01W";
constexpr uint32_t kSaveVersionsDS[] = {
    0x04
};

static SongIdentifier DecodeSongIdentifier(ByteReader& reader)
{
    const bool songHasData = reader.Read<uint8_t>() != 0;

    const std::string name = reader.ReadString(8);

    constexpr auto unknown01 = 0x0F;
    reader.Skip(unknown01);

    const auto songStartAddress = reader.Read<uint32_t>();
    const auto songLength = reader.Read<uint32_t>();

    constexpr auto unknown02 = 0x08;
    reader.Skip(unknown02);

    return {
        .songHasData = songHasData,
        .name = name,
        .songStartAddress = songStartAddress,
        .songLength = songLength
    };
}

static bool CheckHeaderVersion(const uint32_t version)
{
    const auto foundVersion = std::ranges::find(kSaveVersionsDS, version);
    return foundVersion != std::end(kSaveVersionsDS);
}

SaveFile::SaveFile(const std::span<const std::byte> data)
{
    ByteReader reader(data);
    const auto checksum = reader.Read<uint32_t>();
    const auto signature = reader.ReadString(4);
    const auto version = reader.Read<uint32_t>();

    // Sanity check, is this a save for the right app.
    isValid = signature == kFileSignature;

    // Checks the header's checksum. The first four bytes of the save file (u32) should be the value of all other bytes
    // in the header area added together/summed.
    if (const auto sum = reader.SumBytes(0x04, 0x1C4 - 0x04);
        checksum != sum)
    {
        std::cerr << std::format("Header checksum mismatch. Expected: 0x{:08X}, Calculated: 0x{:08X}.\n",
                                 checksum, sum);
        isValid = false;
    }

    if (!CheckHeaderVersion(version))
    {
        std::cerr << std::format(
            "WARNING: Save file format version {} is not officially supported by this tool. Trying to parse anyways.\n",
            version);
    }

    // Only continue if the file has the proper signature, and a valid checksum.
    if (isValid)
    {
        std::vector<SongIdentifier> songIdentifiers;
        songIdentifiers.reserve(kNumberOfSongs);
        // Parse each stored song one at a time.
        for (int i = 0; i < kNumberOfSongs; ++i)
        {
            songIdentifiers.emplace_back(DecodeSongIdentifier(reader));
            const auto& songIdentifier = songIdentifiers.back();

            std::cout << std::format("Song identifier: {}{}\n", songIdentifier.name,
                                     songIdentifier.songHasData ? "" : ", no data!");
        }

        for (const auto& identifier : songIdentifiers)
        {
            if (!identifier.songHasData) continue;
            DecodeSongData(reader, identifier);
        }
    }
}

bool SaveFile::IsValid() const
{
    return isValid;
}
