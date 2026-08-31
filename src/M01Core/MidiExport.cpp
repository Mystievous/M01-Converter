//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 8/28/2026
//

#include "M01Core/MidiExport.h"

#include <format>
#include <iostream>

#include "M01Core/InstrumentHelper.h"
#include "M01Core/SaveStructure.h"
#include "MidiFile.h"

// One quarter note is 4 notes/steps in the app. This is denoted by visible vertical lines in the UI.
constexpr int kTicksPerQuarter = 480;

// A step is one 16th note, or one grid position you can place a note on in the UI.
// Steps can be further divided into 4 sub-positions, but *only* for the ends of notes, never the start.
constexpr int kTicksPerStep = kTicksPerQuarter / 4;
constexpr int kTicksPerSubStep = kTicksPerStep / 4;

// The app always trims the ends of notes by this much
constexpr int kNoteEndPadding = 10;

// MIDI Track 0 is used for meta information like tempo.
// MIDI Tracks 1-8 are then used for instruments 1-8.
constexpr int kMetaTrackIndex = 0;

constexpr int kCcBankMsb = 0x00;
constexpr int kCcBankLsb = 0x20;
constexpr int kCcVolume = 0x07;
constexpr int kCcPan = 0x0A;
constexpr int kCcAttack = 0x49;
constexpr int kCcRelease = 0x48;

// "Swing" is a percentage that represents the amount of the two-step tick period that's given to the first step in it.
constexpr int kNoSwing = 50;
constexpr int kTicksPerSwingPair = kTicksPerStep * 2;

// Length of the stretched (first) step of a swing pair.
// The squeezed step gets the remainder, as a complete pair always is exactly kTicksPerSwingPair
static int StretchedStepTicks(const int swing)
{
    return (kTicksPerStep * swing + kNoSwing / 2) / kNoSwing;
}

// Steps are normally kTicksPerStep ticks long, but swing adjusts it.
// This rescales a step position into the given "actual" length for the step.
static int ScaleIntoStep(const int positionInStep, const int targetStepLength)
{
    return positionInStep * targetStepLength / kTicksPerStep;
}

// Maps non-swing ticks to their proper swing positions.
// Swing steps comes in pairs, where the first step is stretched, and the second one is squeezed (delayed start).
//     i.e. only the "middle" point of the two steps moves with the swing percentage.
//
// Positions inside a step interpolate along the step's rounded length, and truncate. This matches the 3DS export.
static int TicksWithSwing(const int swing, const int ticks)
{
    const auto positionInPair = ticks % kTicksPerSwingPair;
    const auto pairStart = ticks - positionInPair;
    const auto stretchedLength = StretchedStepTicks(swing);

    if (positionInPair < kTicksPerStep)
    {
        return pairStart + ScaleIntoStep(positionInPair, stretchedLength);
    }

    const auto squeezedLength = kTicksPerSwingPair - stretchedLength;
    const auto positionInSecondStep = positionInPair - kTicksPerStep;
    const auto squeezeStart = pairStart + stretchedLength;
    return squeezeStart + ScaleIntoStep(positionInSecondStep, squeezedLength);
}

// Complete swing pairs must have two steps total. Patterns with odd step counts must end with a stretched step.
static int MeasureTicks(const int steps, const int swing)
{
    return steps / 2 * kTicksPerSwingPair + steps % 2 * StretchedStepTicks(swing);
}

static std::vector<int> MeasureStartTicks(const SongData& song)
{
    auto startTicks = std::vector<int>{};
    startTicks.reserve(song.measureInfos.size());

    int cumulativeTicks = 0;

    // ReSharper disable once CppUseStructuredBinding
    for (const auto& measure : song.measureInfos)
    {
        const auto steps = measure.steps != 0 ? measure.steps : song.masterInfo.stepsPerMeasure;

        startTicks.push_back(cumulativeTicks);
        cumulativeTicks += MeasureTicks(steps, song.swing);
    }

    return startTicks;
}

smf::MidiFile MakeMidiFile(const SongData& song)
{
    auto midiFile = smf::MidiFile();

    midiFile.setTicksPerQuarterNote(kTicksPerQuarter);
    midiFile.addTracks(song.masterInfo.numTracks);

    // Meta Track
    midiFile.addTrackName(kMetaTrackIndex, 0, song.name);
    midiFile.addTempo(kMetaTrackIndex, 0, song.masterInfo.tempo);

    // Instrument Tracks
    for (int i = 0; i < song.masterInfo.numTracks; ++i)
    {
        midiFile.addTrackName(i + 1, 0, "Track-" + std::to_string(i));
    }

    const auto measureStartTicks = MeasureStartTicks(song);

    uint16_t currentTempo = song.masterInfo.tempo;
    for (int i = 0; i < static_cast<int>(song.measureInfos.size()); ++i)
    {
        // ReSharper disable once CppTooWideScopeInitStatement
        const auto& [tempo, steps] = song.measureInfos[i];

        if (tempo != 0 && tempo != currentTempo)
        {
            currentTempo = tempo;
            midiFile.addTempo(kMetaTrackIndex, measureStartTicks[i], currentTempo);
        }
        else if (tempo == 0 && currentTempo != song.masterInfo.tempo)
        {
            currentTempo = song.masterInfo.tempo;
            midiFile.addTempo(kMetaTrackIndex, measureStartTicks[i], currentTempo);
        }
    }

    for (int t = 0; t < static_cast<int>(song.tracks.size()); ++t)
    {
        const auto& track = song.tracks[t];
        for (int m = 0; m < static_cast<int>(track.measures.size()); ++m)
        {
            if (track.measures[m].has_value())
            {
                const auto& [notes] = *track.measures[m];
                const auto measureStartTick = measureStartTicks[m];

                for (auto& [length, velocity, pitch, startPoint] : notes)
                {
                    const auto startTime = TicksWithSwing(song.swing, startPoint * kTicksPerStep) + measureStartTick;
                    const auto endTime =
                        TicksWithSwing(song.swing, (startPoint * 4 + length + 1) * kTicksPerSubStep)
                        + measureStartTick - kNoteEndPadding;

                    const auto trackId = t + 1;
                    const auto channelId = t;

                    // Adding 0x80 to pitches to match 3DS export format.
                    midiFile.addNoteOn(trackId, startTime, channelId, pitch + 0x80, velocity);
                    midiFile.addNoteOff(trackId, endTime, channelId, pitch + 0x80, velocity);
                }
            }
        }
    }

    midiFile.sortTracks();

    return midiFile;
}

namespace
{
    struct TrackPlayback
    {
        int track = 0;
        uint8_t channel = 0;
        std::optional<uint8_t> bankMsb;
        std::optional<uint8_t> bankLsb;
        std::optional<uint8_t> program;
    };
}

smf::MidiFile MakeExtendedMidiFile(const SongData& song, const std::string& configPath)
{
    InstrumentHelper instrumentHelper;
    instrumentHelper.LoadConfigFile(configPath);

    auto midiFile = smf::MidiFile();

    const auto trackCount = song.tracks.size();

    midiFile.setTicksPerQuarterNote(kTicksPerQuarter);
    midiFile.addTracks(static_cast<int>(trackCount));

    // Meta Track
    midiFile.addTrackName(kMetaTrackIndex, 0, song.name);
    midiFile.addTempo(kMetaTrackIndex, 0, song.masterInfo.tempo);

    // Initialize Instrument Tracks and Info
    std::vector<TrackPlayback> playbacks;
    playbacks.reserve(trackCount);
    std::vector<InstrumentConfig> configs;
    configs.reserve(trackCount);

    for (size_t i = 0; i < trackCount; ++i)
    {
        const auto& instrument = song.tracks[i].instrument;
        const auto name = instrumentHelper.GetInstrumentName(instrument.id);
        const auto track = static_cast<int>(i) + 1;
        const auto channel = static_cast<uint8_t>(i);

        auto config = instrumentHelper.GetInstrumentConfig(name.bank, name.category, name.program);
        const auto playback = TrackPlayback{
            .track = track,
            .channel = config.channel.value_or(channel),
            .bankMsb = config.bankMsb,
            .bankLsb = config.bankLsb,
            .program = config.program
        };

        for (const auto& comparePlayback : playbacks)
        {
            if (comparePlayback.channel != playback.channel)
                continue;

            // Sharing channel is fine if they have the same program change.
            // Only shared channel with different patch info is a problem.
            if (comparePlayback.bankMsb == playback.bankMsb
                && comparePlayback.bankLsb == playback.bankLsb
                && comparePlayback.program == playback.program)
                continue;

            std::cerr << std::format(
                "Song {}, Track {} has the same MIDI channel as Track {}. Any bank/program changes may be mangled.\n",
                song.name, playback.track, comparePlayback.track);
        }

        configs.push_back(std::move(config));
        playbacks.push_back(playback);

        const auto trackName = std::format("{} - {}", name.bank, name.program);

        midiFile.addTrackName(playback.track, 0, trackName);

        if (playback.bankMsb.has_value() || playback.bankLsb.has_value())
        {
            if (!playback.program.has_value())
            {
                std::cerr << std::format("Song {}: Track {} has bank IDs but no program ID. Skipping.\n", song.name,
                                         track);
            }
            else
            {
                midiFile.addController(
                    playback.track,
                    0,
                    playback.channel,
                    kCcBankMsb,
                    playback.bankMsb.value_or(0)
                );
                midiFile.addController(
                    playback.track,
                    0,
                    playback.channel,
                    kCcBankLsb,
                    playback.bankLsb.value_or(0)
                );
            }
        }
        if (playback.program.has_value())
        {
            midiFile.addPatchChange(
                playback.track,
                0,
                playback.channel,
                *playback.program
            );
        }

        // CC Pan: DS Save File stores pan as -5 to 5.
        midiFile.addController(
            playback.track,
            0,
            playback.channel,
            kCcPan,
            InstrumentHelper::MapRange(instrument.panning, -5, 5, 0, 127));
        // CC Volume: DS Save File stores volume as 0 to 127.
        midiFile.addController(
            playback.track,
            0,
            playback.channel,
            kCcVolume,
            instrument.volume
        );

        // CC Attack: DS Save File stores attack as 0 to 15.
        midiFile.addController(
            playback.track,
            0,
            playback.channel,
            kCcAttack,
            InstrumentHelper::MapRange(instrument.attack, 0, 15, 0, 127)
        );

        // CC Release: DS Save File stores release as 0 to 15.
        midiFile.addController(
            playback.track,
            0,
            playback.channel,
            kCcRelease,
            InstrumentHelper::MapRange(instrument.release, 0, 15, 0, 127)
        );
    }

    const auto measureStartTicks = MeasureStartTicks(song);

    uint16_t currentTempo = song.masterInfo.tempo;
    for (int i = 0; i < static_cast<int>(song.measureInfos.size()); i++)
    {
        // ReSharper disable once CppTooWideScopeInitStatement
        const auto& [tempo, steps] = song.measureInfos[i];

        if (tempo != 0 && tempo != currentTempo)
        {
            currentTempo = tempo;
            midiFile.addTempo(kMetaTrackIndex, measureStartTicks[i], currentTempo);
        }
        else if (tempo == 0 && currentTempo != song.masterInfo.tempo)
        {
            currentTempo = song.masterInfo.tempo;
            midiFile.addTempo(kMetaTrackIndex, measureStartTicks[i], currentTempo);
        }
    }

    for (int t = 0; t < static_cast<int>(song.tracks.size()); ++t)
    {
        const auto& track = song.tracks[t];
        const auto& playback = playbacks[t];
        const auto& config = configs[t];

        for (int m = 0; m < static_cast<int>(track.measures.size()); ++m)
        {
            if (track.measures[m].has_value())
            {
                const auto& [notes] = *track.measures[m];
                const auto measureStartTick = measureStartTicks[m];

                for (auto& [length, velocity, pitch, startPoint] : notes)
                {
                    const auto startTime = TicksWithSwing(song.swing, startPoint * kTicksPerStep) + measureStartTick;
                    const auto endTime = TicksWithSwing(song.swing, (startPoint * 4 + length + 1) * kTicksPerSubStep)
                        + measureStartTick - kNoteEndPadding;

                    const auto remappedPitch = InstrumentHelper::RemapNoteNumber(config, pitch);

                    midiFile.addNoteOn(playback.track, startTime, playback.channel, remappedPitch, velocity);
                    midiFile.addNoteOff(playback.track, endTime, playback.channel, remappedPitch, velocity);
                }
            }
        }
    }

    midiFile.sortTracks();

    return midiFile;
}
