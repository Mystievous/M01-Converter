//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/15/2025
//

#ifndef SAVESTRUCTURE_H
#define SAVESTRUCTURE_H

#include <cstdint>
#include <string>
#include <vector>
#include <optional>

constexpr int kNumberOfSongs = 10;
constexpr int kNumberOfInstruments = 8;
constexpr int kNumberOfMeasures = 99;
constexpr int kNumberOfDrumSamples = 12;

enum class KaosMode : uint8_t
{
    MONO, CHD2, CHD3, CHD4, DRUM
};

enum class Key : uint8_t
{
    C, C_SHARP, D, D_SHARP, E, F, F_SHARP, G, G_SHARP, A, A_SHARP, B
};

enum class Scale : uint8_t
{
    CHROMA, IONIAN, DORIAN, PHRYGI, LYDIAN, MIXLYD, AEOLIA, LOCRIA, MBLUES, mBLUES, DIM, COMDIM, MPENTA, mPENTA, RAGA1,
    RAGA2, ARABIA, SPAIN, GYPSY, EGYPT, HAWAII, PELOG, JAPAN, RYUKYU, WHOLE, m3RD, M3RD, FOURTH, FIFTH, OCTAVE
};

enum class DrumPattern : uint8_t
{
    EIGHT_BEAT1, EIGHT_BEAT2, SIXTEEN_BEAT1, SIXTEEN_BEAT2, ROCK1, ROCK2, ROCK3, FUNK, HOUSE1, HOUSE2, ELECTRO, MINIMAL,
    DnB, RnB, HIPHOP, PERC
};

struct PlaybackState
{
    bool hasFX = false;
    bool muted = false;
    bool soloed = false;
    Key kaosKey = Key::C;
    Scale kaosScale = Scale::CHROMA;
    DrumPattern kaosDrumPattern = DrumPattern::EIGHT_BEAT1;
    KaosMode kaosMode = KaosMode::MONO;
    uint8_t keyboardOctave = 0;
};

struct DrumInfo
{
    uint8_t level = 0;
    uint8_t panning = 0;
    int8_t timestretch = 0;
};

struct InstrumentId
{
    uint8_t bank = 0;
    uint8_t category = 0;
    uint8_t program = 0;
};

struct Instrument
{
    InstrumentId id;
    // Copy of the one in the PlaybackState, but MONO (0x00) if it's a drum track.
    KaosMode kaosModeCopy = KaosMode::MONO;
    int8_t kaosVariation = 0;
    uint8_t attack = 0;
    uint8_t release = 0;
    uint8_t volume = 0;
    int8_t panning = 0;
    PlaybackState playbackState;
    std::vector<DrumInfo> drumInfo;
};

enum class ReverbType : uint8_t
{
    Hall,
    Room,
    Sprg
};

struct ReverbInfo
{
    ReverbType type = ReverbType::Hall;
    uint8_t time = 0;
    uint8_t level = 0;
};

struct DelayInfo
{
    bool syncOn = false;
    uint8_t time = 0;
    int8_t panRatio = 0;
    uint8_t feedback = 0;
    uint8_t level = 0;
};

struct MasterInfo
{
    uint8_t numTracks = 0;
    uint8_t numMeasures = 0;
    uint16_t tempo = 0;
    uint8_t swing = 0;
    uint8_t stepsPerMeasure = 0;
};

constexpr auto kMeasureInfoSize = 0x08;

struct MeasureInfo
{
    uint16_t tempo = 0;
    uint8_t steps = 0;
};

constexpr auto kNoteDataSize = 0x04;

struct NoteData
{
    uint8_t length = 0;
    uint8_t velocity = 0;
    int8_t pitch = 0;
    uint8_t startPoint = 0;
};

struct Pattern
{
    std::vector<NoteData> notes;
};

struct Track
{
    Instrument instrument;
    std::vector<std::optional<Pattern>> measures;
};

struct SongIdentifier
{
    bool songHasData;
    std::string name;
    uint32_t songStartAddress;
    uint32_t songLength;
};

enum class FXType : uint8_t
{
    Delay = 0,
    Reverb = 1,
};

struct SongData
{
    std::string name;
    bool hasSolo = false;
    FXType fxType = FXType::Delay;
    bool locked = false;
    uint8_t swing = 0;
    ReverbInfo reverbInfo;
    DelayInfo delayInfo;
    MasterInfo masterInfo;
    std::vector<MeasureInfo> measureInfos;
    std::vector<Track> tracks;
};

struct SaveHeader
{
    const uint32_t checksum;
    const std::string signature; // "M01W"
    const uint32_t version;
};

#endif //SAVESTRUCTURE_H
