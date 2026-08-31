//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 8/27/2026
//

#include "M01Core/SongDecoder.h"

#include <cstdint>
#include <format>
#include <iostream>
#include <optional>
#include <vector>
#include <string>
#include <algorithm>

#include "M01Core/Bits.h"
#include "M01Core/InstrumentHelper.h"

static constexpr std::string_view ToString(const KaosMode kaosMode)
{
    switch (kaosMode)
    {
    case KaosMode::MONO: return "MONO";
    case KaosMode::CHD2: return "CHD2";
    case KaosMode::CHD3: return "CHD3";
    case KaosMode::CHD4: return "CHD4";
    case KaosMode::DRUM: return "DRUM";
    }
    return "UNKNOWN";
}

constexpr std::string_view kKaosKey[] = {
    "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
};

static constexpr std::string_view ToString(const Key key)
{
    const auto index = static_cast<size_t>(key);
    return index < std::size(kKaosKey) ? kKaosKey[index] : "UNKNOWN";
}

constexpr std::string_view kKaosScale[] = {
    "CHROMA", "IONIAN", "DORIAN", "PHRYGI", "LYDIAN", "MIXLYD", "AEOLIA", "LOCRIA", "MBLUES", "mBLUES", "DIM", "COMDIM",
    "MPENTA", "mPENTA", "RAGA1", "RAGA2", "ARABIA", "SPAIN", "GYPSY", "EGYPT", "HAWAII", "PELOG", "JAPAN", "RYUKYU",
    "WHOLE", "m3RD", "M3RD", "4TH", "5TH", "OCTAVE"
};

static constexpr std::string_view ToString(const Scale scale)
{
    const auto index = static_cast<size_t>(scale);
    return index < std::size(kKaosScale) ? kKaosScale[index] : "UNKNOWN";
}

constexpr std::string_view kKaosDrumPattern[] = {
    "8BEAT1", "8BEAT2", "16BEAT1", "16BEAT2", "ROCK1", "ROCK2", "ROCK3", "FUNK", "HOUSE1",
    "HOUSE2", "ELECTRO", "MINIMAL", "D&B", "R&B", "HIPHOP", "PERC"
};

static constexpr std::string_view ToString(const DrumPattern drumPattern)
{
    const auto index = static_cast<size_t>(drumPattern);
    return index < std::size(kKaosDrumPattern) ? kKaosDrumPattern[index] : "UNKNOWN";
}

static constexpr std::string_view ToString(const ReverbType reverbType)
{
    switch (reverbType)
    {
    case ReverbType::Hall: return "Hall";
    case ReverbType::Room: return "Room";
    case ReverbType::Sprg: return "Sprg";
    }
    return "UNKNOWN";
}

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

static Instrument DecodeInstrument(ByteReader& reader, const SongIdentifier& identifier)
{
    const auto bank = reader.Read<uint8_t>();
    const auto category = reader.Read<uint8_t>();
    const auto program = reader.Read<uint8_t>();

    const auto id = InstrumentId{
        .bank = bank,
        .category = category,
        .program = program
    };

    const auto kaosModeCopy = static_cast<KaosMode>(reader.Read<uint8_t>());
    const auto kaosVariation = reader.Read<int8_t>();
    const auto attack = reader.Read<uint8_t>();
    const auto release = reader.Read<uint8_t>();
    const auto volume = reader.Read<uint8_t>();
    const auto panning = reader.Read<int8_t>();

    const auto instrumentHelper = InstrumentHelper{};
    const auto instrumentName = instrumentHelper.GetInstrumentName(id);

    const auto playbackByte = reader.Read<uint8_t>();
    const auto hasFX = bits::Get<0, 1>(playbackByte);
    const auto muted = bits::Get<1, 1>(playbackByte);
    const auto soloed = bits::Get<2, 1>(playbackByte);
    const auto kaosKey = static_cast<Key>(bits::Get<3, 5>(playbackByte));

    const auto kaosScale = static_cast<Scale>(reader.Read<uint8_t>());
    const auto kaosDrumPattern = static_cast<DrumPattern>(reader.Read<uint8_t>());
    const auto kaosMode = static_cast<KaosMode>(reader.Read<uint8_t>());
    const auto keyboardOctave = reader.Read<uint8_t>();
    reader.Skip(0x01);

    if (kaosMode != KaosMode::DRUM && kaosMode != kaosModeCopy)
    {
        std::cerr << std::format("Song {} instrument {} - {} - {} has mismatched kaos modes.\n", identifier.name,
                                 instrumentName.bank, instrumentName.category, instrumentName.program);
    }

    const auto playbackState = PlaybackState{
        .hasFX = hasFX,
        .muted = muted,
        .soloed = soloed,
        .kaosKey = kaosKey,
        .kaosScale = kaosScale,
        .kaosDrumPattern = kaosDrumPattern,
        .kaosMode = kaosMode,
        .keyboardOctave = keyboardOctave
    };

    auto drumInfos = std::vector<DrumInfo>{};
    drumInfos.reserve(kNumberOfDrumSamples);
    for (int i = 0; i < kNumberOfDrumSamples; ++i)
    {
        const auto mixByte = reader.Read<uint8_t>();
        const auto sampleLevel = bits::Get<0, 4>(mixByte);
        const auto samplePanning = bits::Get<4, 4>(mixByte);

        const auto sampleTimestretch = reader.Read<int8_t>();

        drumInfos.emplace_back(sampleLevel, samplePanning, sampleTimestretch);
    }

    reader.Skip(0x11);

    return {
        .id = id,
        .kaosModeCopy = kaosModeCopy,
        .kaosVariation = kaosVariation,
        .attack = attack,
        .release = release,
        .volume = volume,
        .panning = panning,
        .playbackState = playbackState,
        .drumInfo = std::move(drumInfos)
    };
}

namespace
{
    enum class Tag : uint16_t
    {
        End = 0x0000,
        MasterInfo = 0x0201,
        MeasureInfo = 0x0104,
        PatternData = 0x0105,
    };
}

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

    std::vector<Instrument> instruments;
    instruments.reserve(kNumberOfInstruments);

    for (int i = 0; i < kNumberOfInstruments; ++i)
    {
        instruments.emplace_back(DecodeInstrument(reader, identifier));
    }

    const auto hasSolo = bits::Get<0, 1>(reader.Read<uint8_t>());

    reader.Skip(0x03);

    const auto reverbType = static_cast<ReverbType>(reader.Read<uint8_t>());
    const auto reverbTime = reader.Read<uint8_t>();
    const auto reverbLevel = reader.Read<uint8_t>();
    const auto reverbInfo = ReverbInfo{
        .type = reverbType,
        .time = reverbTime,
        .level = reverbLevel
    };
    reader.Skip(0x01);

    const auto delaySyncOn = bits::Get<0, 1>(reader.Read<uint8_t>());
    const auto delayTime = reader.Read<uint8_t>();
    const auto delayPanRatio = reader.Read<int8_t>();
    const auto delayFeedback = reader.Read<uint8_t>();
    const auto delayLevel = reader.Read<uint8_t>();
    const auto delayInfo = DelayInfo{
        .syncOn = delaySyncOn,
        .time = delayTime,
        .panRatio = delayPanRatio,
        .feedback = delayFeedback,
        .level = delayLevel
    };
    reader.Skip(0x03);

    const auto sceneBytes = reader.Read<uint16_t>();
    const auto fxType = bits::Get<0, 1>(sceneBytes) ? FXType::Reverb : FXType::Delay;
    // One empty bit between reverb toggle and locked
    const auto locked = bits::Get<2, 1>(sceneBytes);
    const uint8_t swing = bits::Get<3, 8>(sceneBytes);

    reader.Skip(0x16);

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
            const auto masterSwing = reader.Read<uint8_t>();
            const auto stepsPerMeasure = reader.Read<uint8_t>();
            reader.Skip(0x02);

            if (numTracks > kNumberOfInstruments)
            {
                std::cerr << std::format(
                    "Number of tracks exceeds the maximum for song {}. Value: {}, Maximum: {}\n",
                    identifier.name, numTracks, kNumberOfInstruments
                );
            }

            // MasterInfo comes before tracks in all 3DS and NDS saves.
            // If it comes afterwards, tracks processed before it will be cleared.
            tracks.assign(numTracks, Track{
                              .instrument = {},
                              .measures = std::vector<std::optional<Pattern>>(numMeasures)
                          });

            const auto instrumentCount = std::min<int>(numTracks, static_cast<int>(instruments.size()));
            for (int i = 0; i < instrumentCount; ++i)
            {
                tracks[i].instrument = instruments[i];
            }

            masterInfo = {
                .numTracks = numTracks,
                .numMeasures = numMeasures,
                .tempo = tempo,
                .swing = masterSwing,
                .stepsPerMeasure = stepsPerMeasure
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
                    const auto steps = reader.Read<uint8_t>();
                    measures->emplace_back(tempo, steps);
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
                        const auto noteIntensity = reader.Read<uint8_t>();
                        const auto noteId = reader.Read<uint8_t>();
                        const auto startPoint = reader.Read<uint8_t>();

                        notes.emplace_back(
                            noteLength,
                            noteIntensity * 17 / 2,
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

        if (reader.Position() != chunkEnd)
        {
            std::cerr << std::format(
                "Payload with tag 0x{:04X} at 0x{:X} does not end at the correct address for song {}. Expected: 0x{:X}, Found: 0x{:X}\n",
                static_cast<uint16_t>(chunkTag), chunkStart - 0x04, identifier.name, chunkEnd,
                reader.Position());
        }

        if (chunkTag == Tag::End) break;

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

    // Ensure there is measureInfo entries for all measures present.
    measures->resize(masterInfo->numMeasures);

    return SongData{
        .name = identifier.name,
        .hasSolo = hasSolo,
        .fxType = fxType,
        .locked = locked,
        .swing = swing,
        .reverbInfo = reverbInfo,
        .delayInfo = delayInfo,
        .masterInfo = *masterInfo,
        .measureInfos = std::move(*measures),
        .tracks = std::move(tracks)
    };
}
