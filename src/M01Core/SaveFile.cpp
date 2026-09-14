//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/21/2025
//

#include "M01Core/SaveFile.h"

#include <string>
#include <string_view>
#include <format>
#include <iostream>
#include <cstdint>
#include <vector>
#include <span>
#include <filesystem>

#include "M01Core/FileBytes.h"
#include "M01Core/SaveStructure.h"
#include "M01Core/SongDecoder.h"
#include "M01Core/ByteReader.h"

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
        .hasData = songHasData,
        .name = name,
        .location = songStartAddress,
        .length = songLength,
    };
}

SaveFile::SaveFile(std::span<const std::byte> bytes, const std::filesystem::path& parentDir)
{
    ByteReader reader(bytes);
    const auto checksum = reader.Read<uint32_t>();
    const auto signature = reader.ReadString(4);
    const auto version = reader.Read<uint32_t>();

    isValid = true;

    if (signature != kFileSignature)
    {
        std::cerr << std::format("ERROR: Save file is missing `M01W` signature.");
        isValid = false;
        return;
    }

    const auto saveFormat = FormatFromSaveVersion(version);
    if (!saveFormat.has_value())
    {
        std::cerr << std::format("ERROR: Save file format version {} is unfamiliar to this tool.\n", version);
        isValid = false;
        return;
    }

    // Verifies the header's checksum.
    constexpr auto checksumStart = 0x04;
    const auto checksumEnd = saveFormat == SaveFormat::M01 ? 0x1C4 : 0x04D;
    if (reader.Has(checksumEnd - checksumStart, checksumStart))
    {
        const auto sum = reader.SumBytes(checksumEnd - checksumStart, checksumStart);
        if (checksum != sum)
        {
            std::cerr << std::format("Header checksum mismatch. Expected: 0x{:08X}, Calculated: 0x{:08X}.\n", checksum,
                                     sum);
            isValid = false;
            return;
        }
    }
    else
    {
        std::cerr << "Save file is incomplete, possibly truncated." << std::endl;
        isValid = false;
        return;
    }

    // M01 for the NDS has a capacity of 10 saved songs.
    // M01D for the 3DS does not have this limit.
    auto numberOfSongs = kM01NumberOfSongs;

    if (saveFormat == SaveFormat::M01D)
    {
        reader.Skip(0x08);
        const auto numSongs = reader.Read<uint32_t>();
        reader.Skip(0x21);
        const auto numSongsCopy = reader.Read<uint32_t>();
        reader.Skip(0x10);
        if (numSongs != numSongsCopy)
        {
            std::cerr << std::format("Conflicting saved number of songs: first {}, second {}\nUsing the first, {}",
                                     numSongs, numSongsCopy, numSongs);
        }
        numberOfSongs = numSongs;

        if (!reader.Has(static_cast<size_t>(numberOfSongs) * kSongIdentifierLength, 0x4D))
        {
            std::cerr << "Save file is incomplete, possibly truncated." << std::endl;
            isValid = false;
            return;
        }
    }


    std::vector<SongIdentifier> songIdentifiers;
    songIdentifiers.reserve(static_cast<size_t>(numberOfSongs));
    // Parse each stored song one at a time.
    for (int i = 0; i < numberOfSongs; ++i)
    {
        songIdentifiers.emplace_back(DecodeSongIdentifier(reader));
    }

    songs = std::vector<std::optional<SongData>>{};
    songs.reserve(numberOfSongs);

    for (const auto& identifier : songIdentifiers)
    {
        if (!identifier.hasData)
        {
            songs.push_back(std::nullopt);
            continue;
        }

        std::vector<std::byte> songBytes;
        ByteReader songReader = reader;

        if (saveFormat == SaveFormat::M01D)
        {
            const auto songPath = parentDir / std::format("M01Dn_{:08x}", identifier.location);
            songBytes = ReadWholeFile(songPath);
            songReader = ByteReader(songBytes);
        }

        const SongSource songSource{.name = identifier.name, .songLength = identifier.length};

        const auto song =
            DecodeSongData(songReader, songSource, saveFormat == SaveFormat::M01D ? 0x00 : identifier.location);
        if (song.has_value())
        {
            songs.push_back(*song);
        }
        else
        {
            std::cerr << std::format("Song {} was unable to be read, skipping...", identifier.name);
            songs.push_back(std::nullopt);
        }
    }
}

bool SaveFile::IsValid() const
{
    return isValid;
}
