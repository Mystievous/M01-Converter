//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/21/2025
//

#include "M01Core/SaveFile.h"

#include <string>
#include <string_view>
#include <iostream>
#include <algorithm>
#include <cstdint>
#include <vector>

#include "M01Core/ByteReader.h"

constexpr std::string_view kFileSignature = "M01W";
constexpr uint32_t kDSVersions[] = {
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

static void DecodeSongData(ByteReader& reader, const SongIdentifier& identifier)
{
    reader.Seek(identifier.songStartAddress);
    const auto songChecksum = reader.Read<uint32_t>();

    const auto calculatedChecksum = reader.SumBytes(identifier.songStartAddress + 0x04,
                                                    identifier.songLength - 0x04);
    if (songChecksum != calculatedChecksum)
    {
        std::cerr << std::hex << "Song checksum mismatch for " << identifier.name << ". Expected: `\\x" <<
            songChecksum << "`, Calculated: `\\x" << calculatedChecksum << "`." << std::dec << std::endl;
    }

    const auto songVersion = reader.Read<uint32_t>();
    reader.Skip(0x1E8);
    if (const auto label = reader.ReadString(4); label != "song")
    {
        std::cerr << "Song label is not `song`, instead: " << label << std::endl;
    }
    const auto songDataLength = reader.Read<uint32_t>();
    const auto songDataVersion = reader.Read<uint32_t>();
    reader.Skip(4);
}

static bool check_header_version(const uint32_t version)
{
    const auto foundVersion = std::ranges::find(kDSVersions, version);
    return foundVersion != std::end(kDSVersions);
}

SaveFile::SaveFile(std::span<const std::byte> data)
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
        std::cerr << std::hex << std::uppercase << "Header checksum mismatch. Expected: `\\x" << checksum <<
            "`. Calculated: `\\x" << sum << "`." << std::dec << std::endl;
        isValid = false;
    }

    if (!check_header_version(version))
    {
        std::cerr << "WARNING: Save file format version " << version <<
            " is not officially supported by this tool. Trying to parse anyways." << std::endl;
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

            std::cout << "Song identifier: " << songIdentifier.name;
            if (!songIdentifier.songHasData) std::cout << ", no data!";
            std::cout << std::endl;
        }

        for (const auto& identifier : songIdentifiers)
        {
            if (!identifier.songHasData) continue;
            DecodeSongData(reader, identifier);
        }

        // for (const auto& songIdentifier : header.songIdentifiers)
        // {
        //     if (songIdentifier.songHasData)
        //     {
        //         songs.emplace_back(saveFile, songIdentifier);
        //     }
        // }
    }
}

bool SaveFile::IsValid() const
{
    return isValid;
}
