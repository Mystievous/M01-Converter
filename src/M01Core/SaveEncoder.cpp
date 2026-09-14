//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 9/12/2026
//

#include "M01Core/SaveEncoder.h"
#include "M01Core/Bits.h"
#include "M01Core/ByteWriter.h"
#include "M01Core/SongEncoder.h"

#include <span>
#include <vector>
#include <format>

constexpr size_t RoundUpTo(size_t value, size_t grid)
{
    return (value + grid - 1) & ~(grid - 1);
}

std::vector<std::byte> EncodeSaveIndex(std::span<const SongIdentifier> songEntries, const SaveFormat saveFormat)
{
    ByteWriter writer{};

    auto checksumStart = writer.Position();
    writer.Pad(std::byte{0x00}, sizeof(uint32_t));
    writer.WriteString(kFileSignature, 4);
    writer.Write<uint32_t>(GetSaveVersion(saveFormat));

    if (saveFormat == SaveFormat::M01D)
    {
        writer.Pad(std::byte{0x00}, 0x08);
        writer.Write<uint32_t>(songEntries.size());
        writer.Pad(std::byte{0x00}, 0x21);
        writer.Write<uint32_t>(songEntries.size());
        writer.Pad(std::byte{0x00}, 0x10);

        // M01D checksum does not cover song index entries
        const uint32_t checksum =
            writer.SumBytes(writer.Position() - (checksumStart + sizeof(uint32_t)), checksumStart + sizeof(uint32_t));
        writer.PatchAt<uint32_t>(checksum, checksumStart);
    }

    for (size_t i = 0; i < songEntries.size(); i++)
    {
        const auto& songEntry = songEntries[i];
        writer.Write<uint8_t>(bits::Set<0, 1>(songEntry.hasData, uint8_t{0x00}));
        writer.WriteString(songEntry.name, 8);
        writer.Pad(std::byte{0x00}, 0x0F);
        writer.Write<uint32_t>(songEntry.location);
        writer.Write<uint32_t>(songEntry.length);
        writer.Pad(std::byte{0x00}, 0x08);
    }

    if (saveFormat == SaveFormat::M01)
    {
        // The size for one extra song slot, for some reason
        writer.Pad(std::byte{0x00}, kSongIdentifierLength);

        const uint32_t checksum =
            writer.SumBytes(writer.Position() - (checksumStart + sizeof(uint32_t)), checksumStart + sizeof(uint32_t));
        writer.PatchAt<uint32_t>(checksum, checksumStart);
    }

    return std::move(writer).Take();
}

std::vector<EncodedSaveFile> EncodeSave(std::span<const std::optional<SongData>> songs, const SaveFormat saveFormat)
{

    const auto numSongsWithData =
        std::count_if(songs.begin(), songs.end(), [](const std::optional<SongData>& song) { return song.has_value(); });

    std::vector<EncodedSaveFile> saveFiles;
    if (saveFormat == SaveFormat::M01)
    {
        if (songs.size() != kM01NumberOfSongs)
        {
            throw std::runtime_error(std::format("Tried to make M01 save with {} songs. M01 only supports {}",
                                                 songs.size(), kM01NumberOfSongs));
        }
        // M01 only has one save file, with the index and all the songs in it.
        saveFiles.reserve(1);
    }
    else
    {
        // Songs with data are the first entries, index is the last one.
        saveFiles.reserve(numSongsWithData + 1);
    }

    std::vector<SongIdentifier> songEntries;
    songEntries.reserve(songs.size());

    std::vector<std::optional<std::vector<std::byte>>> songBytes;
    if (saveFormat == SaveFormat::M01)
    {
        songBytes.reserve(songs.size());
    }

    for (size_t i = 0; i < songs.size(); i++)
    {
        const auto& song = songs[i];

        SongIdentifier entry{};

        if (saveFormat == SaveFormat::M01)
        {
            entry.location = kM01FirstSongAddress + (kM01FollowingSongsOffset * i);
        }
        else
        {
            entry.location = kM01DInitialSongID + i;
        }

        if (song.has_value())
        {
            auto encodedSong = EncodeSongData(*song, saveFormat);

            entry.hasData = true;
            entry.name = song->name;
            entry.length = encodedSong.size();

            if (saveFormat == SaveFormat::M01D)
            {
                const auto fileName = std::format("M01Dn_{:08x}", entry.location);
                encodedSong.resize(RoundUpTo(entry.length, kM01DFileSizeRound));
                saveFiles.emplace_back(fileName, std::move(encodedSong));
            }
            else
            {
                songBytes.emplace_back(std::move(encodedSong));
            }
        }
        else
        {
            entry.hasData = false;
            entry.name = std::format("S-{:02}", i + 1);
            entry.length = GetUninitializedSongLength(saveFormat);
            if (saveFormat == SaveFormat::M01)
            {
                songBytes.emplace_back(std::nullopt);
            }
        }

        songEntries.emplace_back(std::move(entry));
    }

    const auto index = EncodeSaveIndex(songEntries, saveFormat);

    ByteWriter indexWriter{};

    indexWriter.WriteBytes(index);

    if (saveFormat == SaveFormat::M01)
    {
        indexWriter.Pad(std::byte{0xFF}, 0x40);
        indexWriter.WriteBytes(index);
        indexWriter.PadUntil(std::byte{0xFF}, kM01HeaderEndAddress);
        indexWriter.WriteString(kM01HeaderEndString, 4);

        for (size_t i = 0; i < songBytes.size(); i++)
        {
            if (i != 0)
            {
                indexWriter.PadUntil(std::byte{0xFF}, songEntries[i].location);
            }

            const auto& bytes = songBytes[i];

            if (bytes.has_value())
            {
                indexWriter.WriteBytes(*bytes);
            }
        }

        indexWriter.PadUntil(std::byte{0xFF}, kM01StaticFileSize);
        saveFiles.emplace_back("M01_Save.sav", std::move(indexWriter).Take());
    }
    else
    {
        indexWriter.PadUntil(std::byte{0x00}, RoundUpTo(indexWriter.Position(), kM01DFileSizeRound));
        saveFiles.emplace_back("M01Dn_00000000", std::move(indexWriter).Take());
    }

    return saveFiles;
}
