//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 8/28/2026
//

#include "M01Core/MidiExport.h"

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

// "Swing" is a percentage that represents the amount of the two-step tick period that's given to the first step in it.
constexpr int kNoSwing = 50;
constexpr int kTicksPerSwingPair = kTicksPerStep * 2;

// Properly adjusts a tick position from the given percentage, for swing calculations.
static int ScaleTicks(const int ticks, const int percent)
{
    return (ticks * percent + kNoSwing / 2) / kNoSwing;
}

// Maps non-swing ticks to their proper swing positions.
// Swing steps comes in pairs, where the first step is stretched, and the second one is squeezed (delayed start).
// i.e. only the "middle" point of the two steps moves with the swing percentage.
static int TicksWithSwing(const int swing, const int ticks)
{
    const int positionInPair = ticks % kTicksPerSwingPair;
    const int pairStart = ticks - positionInPair;

    if (positionInPair < kTicksPerStep)
    {
        return pairStart + ScaleTicks(positionInPair, swing);
    }
    return pairStart + ScaleTicks(kTicksPerStep, swing) + ScaleTicks(
        positionInPair - kTicksPerStep, 100 - swing);
}

// Complete swing pairs must have two steps total. Patterns with odd step counts must end with a stretched step.
static int MeasureTicks(const int steps, const int swing)
{
    return steps / 2 * kTicksPerSwingPair
        + steps % 2 * ScaleTicks(kTicksPerStep, swing);
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
                    const int startTime = TicksWithSwing(song.swing, startPoint * kTicksPerStep) + measureStartTick;
                    const int endTime =
                        TicksWithSwing(song.swing, (startPoint * 4 + length + 1) * kTicksPerSubStep)
                        + measureStartTick - kNoteEndPadding;

                    // Adding 0x80 to pitches to match 3DS export format.
                    midiFile.addNoteOn(t + 1, startTime, t, pitch + 0x80, velocity);
                    midiFile.addNoteOff(t + 1, endTime, t, pitch + 0x80, velocity);
                }
            }
        }
    }

    midiFile.sortTracks();

    return midiFile;
}

// smf::MidiFile MakeExtendedMidiFile(const SongData& song, const std::string& configPath)
// {
//     InstrumentHelper instrumentHelper;
//     instrumentHelper.LoadConfigFile(configPath);
//
//     auto midiFile = smf::MidiFile();
//
//     midiFile.setTicksPerQuarterNote(kTicksPerQuarter);
//     midiFile.addTracks(song.masterInfo.numTracks);
//
//     // Meta Track
//     midiFile.addTrackName(kMetaTrackIndex, 0, song.name);
//     midiFile.addTempo(kMetaTrackIndex, 0, song.masterInfo.tempo);
//
//     // Initialize Instrument Tracks and Info
//     InstrumentPlaybackInfo instrumentPlaybackInfo[kNumberOfInstruments];
//     InstrumentConfig instrumentConfigs[kNumberOfInstruments];
//     for (int i = 0; i < kNumberOfInstruments; ++i)
//     {
//         // midiFile.addTrackName(i + 1, 0, "Track-" + std::to_string(i));
//         const auto& instrument = header.instruments[i];
//         const auto& [bank, subBank, program] = instrumentHelper.GetInstrumentName(
//             instrument.bank,
//             instrument.subBank,
//             instrument.program);
//
//         instrumentPlaybackInfo[i].track = i + 1;
//         instrumentPlaybackInfo[i].channel = i;
//
//         instrumentConfigs[i] = instrumentHelper.GetInstrumentConfig(bank, subBank, program);
//
//         if (instrumentConfigs[i].channel.has_value())
//         {
//             instrumentPlaybackInfo[i].channel = instrumentConfigs[i].channel.value();
//         }
//
//         if (instrumentConfigs[i].bank.has_value())
//         {
//             instrumentPlaybackInfo[i].bank = instrumentConfigs[i].bank.value();
//         }
//         else
//         {
//             instrumentPlaybackInfo[i].bank = 0;
//         }
//
//         if (instrumentConfigs[i].subBank.has_value())
//         {
//             instrumentPlaybackInfo[i].subBank = instrumentConfigs[i].subBank.value();
//         }
//         else
//         {
//             instrumentPlaybackInfo[i].subBank = 0;
//         }
//
//         if (instrumentConfigs[i].program.has_value())
//         {
//             instrumentPlaybackInfo[i].program = instrumentConfigs[i].program.value();
//         }
//
//         const auto& info = instrumentPlaybackInfo[i];
//         std::string name = bank;
//         name += " - ";
//         name += program;
//
//         midiFile.addTrackName(info.track, 0, name);
//         midiFile.addController(
//             info.track,
//             0,
//             info.channel,
//             0x00,
//             instrumentPlaybackInfo[i].bank);
//         midiFile.addController(
//             info.track,
//             0,
//             info.channel,
//             0x20,
//             instrumentPlaybackInfo[i].subBank);
//         midiFile.addPatchChange(info.track, 0, info.channel, info.program);
//
//         // CC Pan: DS Save File stores pan as -5 to 5.
//         midiFile.addController(
//             info.track,
//             0,
//             info.channel,
//             0x0A,
//             InstrumentHelper::MapRange(instrument.panning, -5, 5, 0, 127));
//         // CC Volume: DS Save File stores volume as 0 to 127.
//         midiFile.addController(info.track, 0, info.channel, 0x07, instrument.volume);
//
//         // CC Attack: DS Save File stores attack as 0 to 15.
//         midiFile.addController(
//             info.track,
//             0,
//             info.channel,
//             0x49,
//             InstrumentHelper::MapRange(instrument.attack, 0, 15, 0, 127));
//
//         // CC Release: DS Save File stores release as 0 to 15.
//         midiFile.addController(
//             info.track,
//             0,
//             info.channel,
//             0x48,
//             InstrumentHelper::MapRange(instrument.release, 0, 15, 0, 127));
//     }
//
//     int cumulativeTickCount = 0;
//     uint16_t currentTempo = header.masterInfo.tempo;
//     for (int i = 0; i < kNumberOfMeasures; i++)
//     {
//         const auto& measureInfo = header.measureInfo[i];
//         const auto& measure = measures[i];
//         if (measureInfo.tempo != 0 && measureInfo.tempo != currentTempo)
//         {
//             currentTempo = measureInfo.tempo;
//             midiFile.addTempo(kMetaTrackIndex, cumulativeTickCount, currentTempo);
//         }
//         else if (measureInfo.tempo == 0 && currentTempo != header.masterInfo.tempo)
//         {
//             currentTempo = header.masterInfo.tempo;
//             midiFile.addTempo(kMetaTrackIndex, cumulativeTickCount, currentTempo);
//         }
//         for (int j = 0; j < kNumberOfInstruments; ++j)
//         {
//             const auto& info = instrumentPlaybackInfo[j];
//             const auto& trackHeader = measure.trackHeaders[j];
//             const auto& noteData = measure.noteData[j];
//
//             for (int k = 0; k < trackHeader.numberOfNotes; ++k)
//             {
//                 const auto& [length, velocity, noteId, startPoint] = noteData[k];
//                 const int startTime = TicksToSwing(startPoint * kTicksPerStep);
//                 const int endTime = TicksToSwing((startPoint * 4 + length + 1) * kTicksPerStep / 4) +
//                     cumulativeTickCount - kNoteEndPadding;
//
//                 const uint8_t& remappedNoteId = InstrumentHelper::RemapNoteNumber(instrumentConfigs[j], noteId);
//
//                 midiFile.addNoteOn(info.track, startTime + cumulativeTickCount, info.channel, remappedNoteId,
//                                    velocity * 17 / 2);
//                 midiFile.addNoteOff(info.track, endTime, info.channel, remappedNoteId, velocity * 17 / 2);
//             }
//         }
//         int measureSteps;
//         if (measureInfo.steps == 0)
//         {
//             measureSteps = header.masterInfo.stepsPerMeasure;
//         }
//         else
//         {
//             measureSteps = measureInfo.steps;
//         }
//
//         cumulativeTickCount += measureSteps * kTicksPerStep;
//         if (measureSteps % 2 == 1)
//         {
//             const int diff = TicksToSwing(2 * kTicksPerStep) - TicksToSwing(1 * kTicksPerStep);
//             cumulativeTickCount += kTicksPerStep - diff;
//         }
//     }
//
//     for (int i = 0; i < kNumberOfInstruments; i++)
//     {
//         midiFile.sortTrack(i + 1);
//     }
//
//     return midiFile;
// }
