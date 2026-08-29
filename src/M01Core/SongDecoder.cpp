//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 8/27/2026
//

#include "M01Core/SongDecoder.h"

#include <format>
#include <iostream>
#include <optional>
#include <vector>

constexpr std::string_view kSongLabel = "song";
constexpr uint32_t kSongVersionsDS[] = {
    0x04
};

constexpr uint32_t kSongDataVersionsDS[] = {
    0x01
};

static bool CheckSongVersion(const uint32_t version)
{
    const auto foundVersion = std::ranges::find(kSongVersionsDS, version);
    return foundVersion != std::end(kSongVersionsDS);
}

static bool CheckSongDataVersion(const uint32_t version)
{
    const auto foundVersion = std::ranges::find(kSongDataVersionsDS, version);
    return foundVersion != std::end(kSongDataVersionsDS);
}

enum class Tag : uint16_t
{
    End = 0x0000,
    MasterInfo = 0x0201,
    MeasureInfo = 0x0104,
    PatternData = 0x0105,
};

std::optional<SongData> DecodeSongData(ByteReader& reader, const SongIdentifier& identifier)
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

    // ReSharper disable once CppTooWideScopeInitStatement
    const auto songVersion = reader.Read<uint32_t>();

    if (!CheckSongVersion(songVersion))
    {
        std::cerr << std::format("Song {} has an unsupported song version, {}. Attempting to parse anyways.\n",
                                 identifier.name, songVersion);
    }

    reader.Skip(0x1E8);
    if (const auto label = reader.ReadString(4); label != kSongLabel)
    {
        std::cerr << std::format("Song label for {} is not `song`, instead: {}\n", identifier.name, label);
    }

    const auto songDataStart = reader.Position();
    const auto songDataLength = reader.Read<uint32_t>(); // Length includes its own bytes
    // ReSharper disable once CppTooWideScopeInitStatement
    const auto songDataVersion = reader.Read<uint32_t>();

    if (!CheckSongDataVersion(songDataVersion))
    {
        std::cerr << std::format("Song {} has an unsupported song data version, {}. Attempting to parse anyways.\n",
                                 identifier.name, songDataVersion);
    }

    const auto songEndAddress = identifier.songStartAddress + identifier.songLength;

    if (songEndAddress != songDataStart + songDataLength)
    {
        std::cerr << std::format("Mismatch in song length values for song {}. File Header: 0x{:X}, Song Data: 0x{:X}\n",
                                 identifier.name, songEndAddress, songDataStart + songDataLength);
    }

    reader.Skip(0x04);

    std::optional<MasterInfo> masterInfo;
    std::optional<std::vector<MeasureInfo>> measures;
    std::vector<Track> tracks;

    while (reader.Position() < songEndAddress)
    {
        const auto chunkTag = static_cast<Tag>(reader.Read<uint16_t>());
        const auto chunkLength = reader.Read<uint16_t>();
        const auto chunkStart = reader.Position();
        const auto chunkEnd = chunkStart + chunkLength;

        if (chunkTag == Tag::MasterInfo)
        {
            if (masterInfo.has_value())
            {
                std::cerr << std::format(
                    "Found more than one Master Info block in song {}. Overwriting previous ones.\n",
                    identifier.name
                );
            }
            const auto numTracks = reader.Read<uint8_t>();
            const auto numMeasures = reader.Read<uint8_t>();
            const auto tempo = reader.Read<uint16_t>();
            const auto swing = reader.Read<uint8_t>();
            const auto step = reader.Read<uint8_t>();
            reader.Skip(0x02);

            if (numTracks > kNumberOfInstruments)
            {
                std::cerr << std::format(
                    "Number of tracks exceeds the maximum for song {}. Value: {}, Maximum: {}\n",
                    identifier.name, numTracks, kNumberOfInstruments
                );
            }
            std::cout << std::format(
                "Number of Tracks: {}\nNumber of Measures: {}\nTempo: {}\nSwing: {}\nStep: {}\n",
                numTracks, numMeasures, tempo,
                swing, step
            );

            // MasterInfo comes before tracks in all 3DS and NDS saves.
            // If it comes afterwards, tracks processed before it will be cleared.
            tracks.assign(numTracks, Track{
                              .measures = std::vector<std::optional<Pattern>>(numMeasures)
                          });

            masterInfo = {
                .numTracks = numTracks,
                .numMeasures = numMeasures,
                .tempo = tempo,
                .swing = swing,
                .step = step
            };
        }
        else if (chunkTag == Tag::MeasureInfo)
        {
            if (measures.has_value())
            {
                std::cerr << std::format(
                    "Found more than one Measure Info block in song {}. Overwriting previous ones.\n",
                    identifier.name
                );
            }
            const auto measureCount = chunkLength / kMeasureInfoSize;
            if (chunkLength % kMeasureInfoSize != 0)
            {
                std::cerr << std::format(
                    "MeasureInfo chunk length is not a multiple of the defined size for song {}. Length: {}, Defined Size: {}\n",
                    identifier.name, chunkLength, kMeasureInfoSize
                );
                reader.Seek(chunkEnd);
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

                measures = std::vector<MeasureInfo>{};
                measures->reserve(measureCount);

                for (int i = 0; i < measureCount; ++i)
                {
                    const auto tempo = reader.Read<uint16_t>();
                    const auto step = reader.Read<uint8_t>();
                    measures->emplace_back(tempo, step);
                    reader.Skip(0x05);
                }
            }
        }
        else if (chunkTag == Tag::PatternData)
        {
            if (!masterInfo.has_value())
            {
                std::cerr << std::format(
                    "PatternData block found before MasterInfo for song {}. Skipping.\n",
                    identifier.name);
                reader.Seek(chunkEnd);
            }
            else
            {
                const auto measureNumber = reader.Read<uint8_t>();
                const auto trackNumber = reader.Read<uint8_t>();
                const auto numberOfNotes = reader.Read<uint16_t>();

                if (measureNumber >= masterInfo->numMeasures)
                {
                    std::cerr << std::format(
                        "In song {}, pattern at 0x{:X} has invalid measure number, {}. Skipping.\n",
                        identifier.name, chunkStart, measureNumber);
                    reader.Seek(chunkEnd);
                }
                else if (trackNumber >= masterInfo->numTracks)
                {
                    std::cerr << std::format(
                        "In song {}, pattern at 0x{:X} has invalid track number, {}. Skipping.\n",
                        identifier.name, chunkStart, trackNumber);
                    reader.Seek(chunkEnd);
                }
                else if (const auto payloadLength = numberOfNotes * kNoteDataSize; 0x04 + payloadLength != chunkLength)
                {
                    std::cerr << std::format(
                        "In song {}, pattern at measure {}, track {} has an invalid number of notes. Skipping.\n",
                        identifier.name, measureNumber, trackNumber);
                    reader.Seek(chunkEnd);
                }
                else
                {
                    std::vector<NoteData> notes;
                    notes.reserve(numberOfNotes);

                    for (int i = 0; i < numberOfNotes; ++i)
                    {
                        const auto noteLength = reader.Read<uint8_t>();
                        const auto noteVelocity = reader.Read<uint8_t>();
                        const auto noteId = reader.Read<uint8_t>();
                        const auto startPoint = reader.Read<uint8_t>();

                        notes.emplace_back(
                            noteLength,
                            noteVelocity,
                            noteId - 0x80,
                            startPoint
                        );
                    }

                    auto& cell = tracks[trackNumber].measures[measureNumber];
                    if (cell.has_value())
                    {
                        std::cerr << std::format(
                            "In song {}, found a duplicate pattern at measure {}, track {}. Overwriting previous entry.\n",
                            identifier.name, measureNumber, trackNumber
                        );
                    }
                    cell = {
                        .notes = std::move(notes)
                    };
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

        if (reader.Position() != chunkEnd)
        {
            std::cerr << std::format(
                "Payload with tag 0x{:04X} at 0x{:X} does not end at the correct address for song {}. Expected: 0x{:X}, Found: 0x{:X}\n",
                static_cast<uint16_t>(chunkTag), chunkStart - 0x04, identifier.name, chunkEnd,
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

    if (!masterInfo.has_value())
    {
        std::cerr << std::format("Song {} did not have a MasterInfo block. Aborting.\n", identifier.name);
        return std::nullopt;
    }

    if (!measures.has_value())
    {
        std::cerr << std::format("Song {} did not have a MeasureInfo block. Aborting.\n", identifier.name);
        return std::nullopt;
    }

    return SongData{
        .masterInfo = *masterInfo,
        .measureInfos = std::move(*measures),
        .tracks = std::move(tracks)
    };
}
