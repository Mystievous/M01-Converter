//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 8/27/2026
//

#include "M01Core/SongDecoder.h"

#include <format>
#include <iostream>
#include <optional>
#include <vector>

constexpr auto kMeasureInfoSize = 0x08;

enum class Tag : uint16_t
{
    End = 0x0000,
    MasterInfo = 0x0201,
    MeasureInfo = 0x0104,
    PatternData = 0x0105,
};

static MasterInfo DecodeMasterInfo(ByteReader& reader)
{
    const auto numTracks = reader.Read<uint8_t>();
    const auto numMeasures = reader.Read<uint8_t>();
    const auto tempo = reader.Read<uint16_t>();
    const auto swing = reader.Read<uint8_t>();
    const auto step = reader.Read<uint8_t>();
    reader.Skip(0x02);

    return {
        .numTracks = numTracks,
        .numMeasures = numMeasures,
        .tempo = tempo,
        .swing = swing,
        .step = step
    };
}

void DecodeSongData(ByteReader& reader, const SongIdentifier& identifier)
{
    reader.Seek(identifier.songStartAddress);
    const auto songChecksum = reader.Read<uint32_t>();

    const auto calculatedChecksum = reader.SumBytes(identifier.songStartAddress + 0x04,
                                                    identifier.songLength - 0x04);
    if (songChecksum != calculatedChecksum)
    {
        std::cerr << std::format("Song checksum mismatch for {}. Expected: 0x{:08X}, Calculated: 0x{:08X}.\n",
                                 identifier.name, songChecksum, calculatedChecksum);
    }

    const auto songVersion = reader.Read<uint32_t>();
    reader.Skip(0x1E8);
    if (const auto label = reader.ReadString(4); label != "song")
    {
        std::cerr << std::format("Song label for {} is not `song`, instead: {}\n", identifier.name, label);
    }

    const auto songDataStart = reader.Position();
    const auto songDataLength = reader.Read<uint32_t>(); // Length includes its own bytes
    const auto songDataVersion = reader.Read<uint32_t>();

    const auto songEndAddress = identifier.songStartAddress + identifier.songLength;

    if (songEndAddress != songDataStart + songDataLength)
    {
        std::cerr << std::format("Mismatch in song length values for song {}. File Header: 0x{:X}, Song Data: 0x{:X}\n",
                                 identifier.name, songEndAddress, songDataStart + songDataLength);
    }

    reader.Skip(0x04);

    std::optional<MasterInfo> masterInfo;

    while (reader.Position() < songEndAddress)
    {
        const auto chunkTag = static_cast<Tag>(reader.Read<uint16_t>());
        const auto chunkLength = reader.Read<uint16_t>();
        const auto chunkStart = reader.Position();

        if (chunkTag == Tag::MasterInfo)
        {
            if (masterInfo.has_value())
            {
                std::cerr << std::format("Found two Master Info blocks in song {}\n", identifier.name);
            }
            masterInfo = DecodeMasterInfo(reader);
            if (masterInfo->numTracks > kNumberOfInstruments)
            {
                std::cerr << std::format("Number of tracks exceeds the maximum for song {}. Value: {}, Maximum: {}\n",
                                         identifier.name, masterInfo->numTracks, kNumberOfInstruments);
            }
            std::cout << std::format("Number of Tracks: {}\nNumber of Measures: {}\nTempo: {}\nSwing: {}\nStep: {}\n",
                                     masterInfo->numTracks, masterInfo->numMeasures, masterInfo->tempo,
                                     masterInfo->swing, masterInfo->step);
        }
        else if (chunkTag == Tag::MeasureInfo)
        {
            const auto measureCount = chunkLength / kMeasureInfoSize;
            if (chunkLength % kMeasureInfoSize != 0)
            {
                std::cerr << std::format(
                    "MeasureInfo chunk length is not a multiple of the defined size for song {}. Length: {}, Defined Size: {}\n",
                    identifier.name, chunkLength, kMeasureInfoSize
                );
                reader.Seek(chunkStart + chunkLength);
            }
            else
            {
                if (masterInfo.has_value())
                {
                    if (measureCount != masterInfo->numMeasures)
                    {
                        std::cerr << std::format(
                            "Calculated measure info count does not match Master Info for song {}. Calculated: {}, Stored: {}\n",
                            identifier.name, measureCount, masterInfo->numMeasures);
                    }
                }
                else
                {
                    std::cerr << std::format(
                        "MeasureInfo block found before MasterInfo for song {}. Automatically deriving from chunk length, value: {}\n",
                        identifier.name, measureCount);
                }
                std::vector<MeasureInfo> measures;
                measures.reserve(measureCount);

                for (int i = 0; i < measureCount; ++i)
                {
                    const auto tempo = reader.Read<uint16_t>();
                    const auto step = reader.Read<uint8_t>();
                    measures.emplace_back(tempo, step);
                    reader.Skip(0x05);
                }
            }
        }
        else
        {
            reader.Skip(chunkLength);
        }

        // std::cout << std::hex << std::uppercase << "Tag: " << static_cast<uint16_t>(tag) << std::dec << std::endl;
        // std::cout << std::hex << std::uppercase << "Payload Start: " << payloadStart - 0x04 << std::dec << std::endl;
        // std::cout << std::hex << std::uppercase << "Length: " << length << std::dec << std::endl;

        if (reader.Position() != chunkStart + chunkLength)
        {
            std::cerr << std::format(
                "Payload with tag 0x{:04X} at 0x{:X} does not end at the correct address for song {}. Expected: 0x{:X}, Found: 0x{:X}\n",
                static_cast<uint16_t>(chunkTag), chunkStart - 0x04, identifier.name, chunkStart + chunkLength,
                reader.Position());
        }

        if (chunkTag == Tag::End) break;

        // std::cout << std::endl;
    }
    if (reader.Position() != songEndAddress)
    {
        std::cerr << std::format("Song {} does not end at the correct address. Expected: 0x{:X}, Found: 0x{:X}\n",
                                 identifier.name, songEndAddress, reader.Position());
    }
}
