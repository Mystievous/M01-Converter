//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 9/12/2026
//

#include "M01Core/SongEncoder.h"
#include "M01Core/ByteWriter.h"
#include "M01Core/SaveStructure.h"
#include "M01Core/Bits.h"

#include <cstdint>
#include <vector>

static void EncodeInstrument(ByteWriter& writer, const Instrument& instrument)
{

    if (instrument.drumInfos.size() != kNumberOfDrumSamples)
    {
        throw std::runtime_error(std::format("Instrument {} - {} - {} has {} drum infos, expected amount is {}",
                                             instrument.id.bank, instrument.id.category, instrument.id.category,
                                             instrument.drumInfos.size(), kNumberOfDrumSamples));
    }

    writer.Write<uint8_t>(instrument.id.bank);
    writer.Write<uint8_t>(instrument.id.category);
    writer.Write<uint8_t>(instrument.id.program);

    writer.Write<KaosMode>(instrument.kaosModeCopy);
    writer.Write<int8_t>(instrument.kaosVariation);
    writer.Write<uint8_t>(instrument.attack);
    writer.Write<uint8_t>(instrument.release);
    writer.Write<uint8_t>(instrument.volume);
    writer.Write<int8_t>(instrument.panning);

    uint8_t playbackByte = 0x00;
    playbackByte = bits::Set<0, 1>(instrument.playbackState.hasFX, playbackByte);
    playbackByte = bits::Set<1, 1>(instrument.playbackState.muted, playbackByte);
    playbackByte = bits::Set<2, 1>(instrument.playbackState.soloed, playbackByte);
    playbackByte = bits::Set<3, 5>(static_cast<uint8_t>(instrument.playbackState.kaosKey), playbackByte);
    writer.Write<uint8_t>(playbackByte);

    writer.Write<Scale>(instrument.playbackState.kaosScale);
    writer.Write<DrumPattern>(instrument.playbackState.kaosDrumPattern);
    writer.Write<KaosMode>(instrument.playbackState.kaosMode);
    writer.Write<uint8_t>(instrument.playbackState.keyboardOctave);

    writer.Pad(std::byte{0x00}, 0x01);

    for (int i = 0; i < kNumberOfDrumSamples; i++)
    {
        const auto& drumInfo = instrument.drumInfos[i];
        uint8_t mixByte = 0x00;
        mixByte = bits::Set<0, 4>(drumInfo.level, mixByte);
        mixByte = bits::Set<4, 4>(drumInfo.panning, mixByte);
        writer.Write<uint8_t>(mixByte);

        writer.Write<int8_t>(drumInfo.timestretch);
    }

    writer.Pad(std::byte{0x00}, 0x11);
}

std::vector<std::byte> EncodeSongData(const SongData& song, const SaveFormat saveFormat)
{
    if (song.tracks.size() != kNumberOfInstruments)
    {
        throw std::runtime_error(std::format("Song {} has {} tracks, expected amount is {}", song.name,
                                             song.tracks.size(), kNumberOfInstruments));
    }

    if (song.measureInfos.size() != kNumberOfMeasures)
    {
        throw std::runtime_error(std::format("Song {} has {} measures, expected amount is {}", song.name,
                                             song.measureInfos.size(), kNumberOfMeasures));
    }

    if (song.masterInfo.numMeasures != kNumberOfMeasures)
    {
        throw std::runtime_error(std::format("Song {} master info counts {} measures, expected amount is {}", song.name,
                                             song.masterInfo.numMeasures, kNumberOfMeasures));
    }

    if (song.masterInfo.numTracks != kNumberOfInstruments)
    {
        throw std::runtime_error(std::format("Song {} master info counts {} tracks, expected amount is {}", song.name,
                                             song.masterInfo.numTracks, kNumberOfInstruments));
    }

    for (size_t i = 0; i < song.tracks.size(); i++)
    {
        if (song.tracks[i].measures.size() != kNumberOfMeasures)
        {
            throw std::runtime_error(std::format("Song {} track {} has {} measures, expected amount is {}", song.name,
                                                 i, song.tracks[i].measures.size(), kNumberOfMeasures));
        }
    }


    ByteWriter writer{};

    // Checksum must be filled in after song data is finished
    const auto checksumStart = 0x00;
    writer.Pad(std::byte{0x00}, sizeof(uint32_t));

    writer.Write<uint32_t>(GetSongVersion(saveFormat));

    for (int i = 0; i < kNumberOfInstruments; i++)
    {
        const auto& track = song.tracks[i];
        EncodeInstrument(writer, track.instrument);
    }

    writer.Write<uint8_t>(bits::Set<0, 1>(song.hasSolo, uint8_t{0x00}));

    writer.Pad(std::byte{0x00}, 0x03);

    writer.Write<ReverbType>(song.reverbInfo.type);
    writer.Write<uint8_t>(song.reverbInfo.time);
    writer.Write<uint8_t>(song.reverbInfo.level);
    writer.Pad(std::byte{0x00}, 0x01);

    writer.Write<uint8_t>(bits::Set<0, 1>(song.delayInfo.syncOn, uint8_t{0x00}));
    writer.Write<uint8_t>(song.delayInfo.time);
    writer.Write<int8_t>(song.delayInfo.panRatio);
    writer.Write<uint8_t>(song.delayInfo.feedback);
    writer.Write<uint8_t>(song.delayInfo.level);
    writer.Pad(std::byte{0x00}, 0x03);

    uint16_t sceneBytes = 0x00;
    sceneBytes = bits::Set<0, 1>(song.fxType == FXType::Reverb, sceneBytes);
    sceneBytes = bits::Set<2, 1>(song.locked, sceneBytes);
    sceneBytes = bits::Set<3, 8>(song.swing, sceneBytes);
    writer.Write<uint16_t>(sceneBytes);

    writer.Pad(std::byte{0x00}, 0x02);

    writer.WriteString(song.sourceSongName, 8);

    writer.Pad(std::byte{0x00}, 0x0C);

    if (saveFormat == SaveFormat::M01D)
    {
        writer.WriteString(kSongMarker, 4);
        writer.Pad(std::byte{0x00}, 0x08);
    }

    writer.WriteString(kSongMarker, 4);
    const auto songDataLengthPos = writer.Position();
    writer.Pad(std::byte{0x00}, sizeof(uint32_t));
    writer.Write<uint32_t>(kSongDataVersion);

    writer.Pad(std::byte{0x00}, 0x04);


    // Master Info
    writer.Write<Tag>(Tag::MasterInfo);
    const auto masterInfoLengthStart = writer.Position();
    writer.Pad(std::byte{0x00}, sizeof(uint16_t));

    writer.Write<uint8_t>(song.masterInfo.numTracks);
    writer.Write<uint8_t>(song.masterInfo.numMeasures);
    writer.Write<uint16_t>(song.masterInfo.tempo);
    writer.Write<uint8_t>(song.masterInfo.swing);
    writer.Write<uint8_t>(song.masterInfo.stepsPerMeasure);
    writer.Pad(std::byte{0x00}, 0x02);

    const uint16_t masterInfoLength = writer.Position() - (masterInfoLengthStart + sizeof(uint16_t));
    writer.PatchAt<uint16_t>(masterInfoLength, masterInfoLengthStart);


    // Measure Info
    writer.Write<Tag>(Tag::MeasureInfo);
    const auto measureInfoLengthStart = writer.Position();
    writer.Pad(std::byte{0x00}, sizeof(uint16_t));

    for (const auto& measure : song.measureInfos)
    {
        writer.Write<uint16_t>(measure.tempo);
        writer.Write<uint8_t>(measure.steps);
        writer.Pad(std::byte{0x00}, 0x05);
    }

    const uint16_t measureInfoLength = writer.Position() - (measureInfoLengthStart + sizeof(uint16_t));
    writer.PatchAt<uint16_t>(measureInfoLength, measureInfoLengthStart);


    // Pattern Data
    for (int i = 0; i < kNumberOfMeasures; i++)
    {
        for (size_t j = 0; j < kNumberOfInstruments; j++)
        {
            const auto& measure = song.tracks[j].measures[i];
            if (!measure.has_value())
                continue;

            writer.Write<Tag>(Tag::PatternData);
            const auto patternDataLengthStart = writer.Position();
            writer.Pad(std::byte{0x00}, sizeof(uint16_t));

            writer.Write<uint8_t>(i);
            writer.Write<uint8_t>(j);
            writer.Write<uint16_t>(measure->notes.size());
            for (const auto& note : measure->notes)
            {
                writer.Write<uint8_t>(note.length);
                writer.Write<uint8_t>((note.velocity * 2 + 16) / 17);
                writer.Write<uint8_t>(note.pitch + 0x80);
                writer.Write<uint8_t>(note.startPoint);
            }

            const uint16_t patternDataLength = writer.Position() - (patternDataLengthStart + sizeof(uint16_t));
            writer.PatchAt<uint16_t>(patternDataLength, patternDataLengthStart);
        }
    }

    writer.Write<Tag>(Tag::End);
    writer.Write<uint16_t>(0x00);

    const uint32_t songDataLength = writer.Position() - songDataLengthPos;
    writer.PatchAt<uint32_t>(songDataLength, songDataLengthPos);

    const uint32_t checksum =
        writer.SumBytes(writer.Position() - (checksumStart + sizeof(uint32_t)), checksumStart + sizeof(uint32_t));
    writer.PatchAt<uint32_t>(checksum, checksumStart);

    return std::move(writer).Take();
}
