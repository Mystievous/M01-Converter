//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/15/2025
//

#include <stdexcept>
#include <cerrno>
#include <cstring>

#include "M01Core/Song.h"

#include "M01Core/InstrumentHelper.h"
#include "MidiFile.h"

constexpr int kTicksPerQuarter = 480;
constexpr int kTicksPerStep = kTicksPerQuarter / 4;
constexpr int kNoteEndPadding = 10;

enum TrackEnum
{
    META,
    INST1,
    INST2,
    INST3,
    INST4,
    INST5,
    INST6,
    INST7,
    INST8
};

Song::Song(FILE *saveFile, const SongIdentifier &identifier) : identifier(identifier),
                                                               header({})
{
    if (fseek(saveFile, identifier.songStartAddress, SEEK_SET) != 0)
    {
        throw std::runtime_error(std::string("fseek failed: ") + std::strerror(errno));
    }

    if (const size_t readCount = fread(&header, sizeof(SongHeader), 1, saveFile); readCount != 1)
    {
        if (feof(saveFile))
            throw std::runtime_error("Unexpected EOF while reading SongHeader");
        throw std::runtime_error(std::string("fread failed: ") + std::strerror(errno));
    }

    measures.reserve(kNumberOfMeasures);
    for (int i = 0; i < kNumberOfMeasures; ++i)
    {
        measures.emplace_back(saveFile);
    }
}

int Song::TicksToSwing(const int ticks) const
{
    const int periodTicks = ticks % (kTicksPerStep * 2);
    const int noteTicks = ticks % kTicksPerStep;
    const int periodStart = ticks - periodTicks;

    if (periodTicks < kTicksPerStep)
    {
        const int swingTicks = (noteTicks * (header.masterInfo.swing * 2) + 50) / 100;
        return periodStart + swingTicks;
    }

    const int swingTicks = (noteTicks * ((100 - header.masterInfo.swing) * 2) + 50) / 100;
    return periodStart + (kTicksPerStep * (header.masterInfo.swing * 2) + 50) / 100 + swingTicks;
}

smf::MidiFile &Song::MakeMidiFile() const
{
    smf::MidiFile &midiFile = *new smf::MidiFile();

    midiFile.setTicksPerQuarterNote(kTicksPerQuarter);
    midiFile.addTracks(8);

    // Meta Track
    midiFile.addTrackName(META, 0, identifier.name);
    midiFile.addTempo(META, 0, header.masterInfo.tempo);

    // Instrument Tracks
    for (int i = 0; i < kNumberOfInstruments; ++i)
    {
        midiFile.addTrackName(i + 1, 0, "Track-" + std::to_string(i));
    }

    int cumulativeTickCount = 0;
    uint16_t currentTempo = header.masterInfo.tempo;
    for (int i = 0; i < kNumberOfMeasures; i++)
    {
        const auto &measureInfo = header.measureInfo[i];
        const auto &measure = measures[i];
        if (measureInfo.tempo != 0 && measureInfo.tempo != currentTempo)
        {
            currentTempo = measureInfo.tempo;
            midiFile.addTempo(META, cumulativeTickCount, currentTempo);
        }
        else if (measureInfo.tempo == 0 && currentTempo != header.masterInfo.tempo)
        {
            currentTempo = header.masterInfo.tempo;
            midiFile.addTempo(META, cumulativeTickCount, currentTempo);
        }
        for (int j = 0; j < kNumberOfInstruments; ++j)
        {
            const auto &trackHeader = measure.trackHeaders[j];
            const auto &noteData = measure.noteData[j];

            for (int k = 0; k < trackHeader.numberOfNotes; ++k)
            {
                const auto &[length, velocity, noteId, startPoint] = noteData[k];
                const int startTime = TicksToSwing(startPoint * kTicksPerStep);
                const int endTime = TicksToSwing((startPoint * 4 + length + 1) * kTicksPerStep / 4) +
                                    cumulativeTickCount - kNoteEndPadding;

                midiFile.addNoteOn(j + 1, startTime + cumulativeTickCount, j, noteId, velocity * 17 / 2);
                midiFile.addNoteOff(j + 1, endTime, j, noteId, velocity * 17 / 2);
            }
        }
        int measureSteps;
        if (measureInfo.step == 0)
        {
            measureSteps = header.masterInfo.step;
        }
        else
        {
            measureSteps = measureInfo.step;
        }

        cumulativeTickCount += measureSteps * kTicksPerStep;
        if (measureSteps % 2 == 1)
        {
            const int diff = TicksToSwing(2 * kTicksPerStep) - TicksToSwing(1 * kTicksPerStep);
            cumulativeTickCount += kTicksPerStep - diff;
        }
    }

    for (int i = 0; i < kNumberOfInstruments; i++)
    {
        midiFile.sortTrack(i + 1);
    }

    return midiFile;
}

smf::MidiFile &Song::MakeExtendedMidiFile(const std::string &configPath) const
{
    InstrumentHelper instrumentHelper;
    instrumentHelper.LoadConfigFile(configPath);

    smf::MidiFile &midiFile = *new smf::MidiFile();

    midiFile.setTicksPerQuarterNote(kTicksPerQuarter);
    midiFile.addTracks(8);

    // Meta Track
    midiFile.addTrackName(META, 0, identifier.name);
    midiFile.addTempo(META, 0, header.masterInfo.tempo);

    // Initialize Instrument Tracks and Info
    InstrumentPlaybackInfo instrumentPlaybackInfo[kNumberOfInstruments];
    InstrumentConfig instrumentConfigs[kNumberOfInstruments];
    for (int i = 0; i < kNumberOfInstruments; ++i)
    {
        // midiFile.addTrackName(i + 1, 0, "Track-" + std::to_string(i));
        const auto &instrument = header.instruments[i];
        const auto &[bank, subBank, program] = instrumentHelper.GetInstrumentName(
            instrument.bank,
            instrument.subBank,
            instrument.program);

        instrumentPlaybackInfo[i].track = i + 1;
        instrumentPlaybackInfo[i].channel = i;

        instrumentConfigs[i] = instrumentHelper.GetInstrumentConfig(bank, subBank, program);

        if (instrumentConfigs[i].channel.has_value())
        {
            instrumentPlaybackInfo[i].channel = instrumentConfigs[i].channel.value();
        }

        if (instrumentConfigs[i].bank.has_value())
        {
            instrumentPlaybackInfo[i].bank = instrumentConfigs[i].bank.value();
        }
        else
        {
            instrumentPlaybackInfo[i].bank = 0;
        }

        if (instrumentConfigs[i].subBank.has_value())
        {
            instrumentPlaybackInfo[i].subBank = instrumentConfigs[i].subBank.value();
        }
        else
        {
            instrumentPlaybackInfo[i].subBank = 0;
        }

        if (instrumentConfigs[i].program.has_value())
        {
            instrumentPlaybackInfo[i].program = instrumentConfigs[i].program.value();
        }

        const auto &info = instrumentPlaybackInfo[i];
        std::string name = bank;
        name += " - ";
        name += program;

        midiFile.addTrackName(info.track, 0, name);
        midiFile.addController(
            info.track,
            0,
            info.channel,
            0x00,
            instrumentPlaybackInfo[i].bank);
        midiFile.addController(
            info.track,
            0,
            info.channel,
            0x20,
            instrumentPlaybackInfo[i].subBank);
        midiFile.addPatchChange(info.track, 0, info.channel, info.program);

        // CC Pan: DS Save File stores pan as -5 to 5.
        midiFile.addController(
            info.track,
            0,
            info.channel,
            0x0A,
            InstrumentHelper::MapRange(instrument.panning, -5, 5, 0, 127));
        // CC Volume: DS Save File stores volume as 0 to 127.
        midiFile.addController(info.track, 0, info.channel, 0x07, instrument.volume);

        // CC Attack: DS Save File stores attack as 0 to 15.
        midiFile.addController(
            info.track,
            0,
            info.channel,
            0x49,
            InstrumentHelper::MapRange(instrument.attack, 0, 15, 0, 127));

        // CC Release: DS Save File stores release as 0 to 15.
        midiFile.addController(
            info.track,
            0,
            info.channel,
            0x48,
            InstrumentHelper::MapRange(instrument.release, 0, 15, 0, 127));
    }

    int cumulativeTickCount = 0;
    uint16_t currentTempo = header.masterInfo.tempo;
    for (int i = 0; i < kNumberOfMeasures; i++)
    {
        const auto &measureInfo = header.measureInfo[i];
        const auto &measure = measures[i];
        if (measureInfo.tempo != 0 && measureInfo.tempo != currentTempo)
        {
            currentTempo = measureInfo.tempo;
            midiFile.addTempo(META, cumulativeTickCount, currentTempo);
        }
        else if (measureInfo.tempo == 0 && currentTempo != header.masterInfo.tempo)
        {
            currentTempo = header.masterInfo.tempo;
            midiFile.addTempo(META, cumulativeTickCount, currentTempo);
        }
        for (int j = 0; j < kNumberOfInstruments; ++j)
        {
            const auto &info = instrumentPlaybackInfo[j];
            const auto &trackHeader = measure.trackHeaders[j];
            const auto &noteData = measure.noteData[j];

            for (int k = 0; k < trackHeader.numberOfNotes; ++k)
            {
                const auto &[length, velocity, noteId, startPoint] = noteData[k];
                const int startTime = TicksToSwing(startPoint * kTicksPerStep);
                const int endTime = TicksToSwing((startPoint * 4 + length + 1) * kTicksPerStep / 4) +
                                    cumulativeTickCount - kNoteEndPadding;

                const uint8_t &remappedNoteId = InstrumentHelper::RemapNoteNumber(instrumentConfigs[j], noteId);

                midiFile.addNoteOn(info.track, startTime + cumulativeTickCount, info.channel, remappedNoteId,
                                   velocity * 17 / 2);
                midiFile.addNoteOff(info.track, endTime, info.channel, remappedNoteId, velocity * 17 / 2);
            }
        }
        int measureSteps;
        if (measureInfo.step == 0)
        {
            measureSteps = header.masterInfo.step;
        }
        else
        {
            measureSteps = measureInfo.step;
        }

        cumulativeTickCount += measureSteps * kTicksPerStep;
        if (measureSteps % 2 == 1)
        {
            const int diff = TicksToSwing(2 * kTicksPerStep) - TicksToSwing(1 * kTicksPerStep);
            cumulativeTickCount += kTicksPerStep - diff;
        }
    }

    for (int i = 0; i < kNumberOfInstruments; i++)
    {
        midiFile.sortTrack(i + 1);
    }

    return midiFile;
}
