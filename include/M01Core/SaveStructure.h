//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/15/2025
//

#ifndef SAVESTRUCTURE_H
#define SAVESTRUCTURE_H

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>
#include <optional>
#include <string_view>

constexpr int kNumberOfInstruments = 8;
constexpr int kNumberOfMeasures = 99;
constexpr int kNumberOfDrumSamples = 12;
constexpr int kM01NumberOfSongs = 10;

// Also shows up on a new save in the "checksum" field of the second/copied header
constexpr std::string_view kM01HeaderEndString = "BFBA";
constexpr uint32_t kM01HeaderEndAddress = 0xFFC;

constexpr uint32_t kM01FirstSongAddress = 0x1000;
constexpr uint32_t kM01FollowingSongsOffset = 0xC000;
constexpr uint32_t kM01StaticFileSize = 0x80000;

constexpr uint32_t kM01DInitialSongID = 0x100000;

// Song data version did not change between M01 and M01D
constexpr uint32_t kSongDataVersion = 0x01;

constexpr std::string_view kFileSignature = "M01W";
constexpr std::string_view kSongMarker = "song";

enum class SaveFormat : uint8_t
{
    M01,
    M01D
};

constexpr uint32_t kM01SongVersion = 0x04;
constexpr uint32_t kM01DSongVersion = 0x05;

constexpr uint32_t GetSongVersion(const SaveFormat saveFormat)
{
    switch (saveFormat)
    {
    case SaveFormat::M01: return kM01SongVersion;
    case SaveFormat::M01D: return kM01DSongVersion;
    }
    throw std::runtime_error("Unknown SaveFormat");
}

constexpr std::optional<SaveFormat> FormatFromSongVersion(const uint32_t version)
{
    switch (version)
    {
    case kM01SongVersion: return SaveFormat::M01;
    case kM01DSongVersion: return SaveFormat::M01D;
    default: return std::nullopt;
    }
}

constexpr uint32_t kM01SaveVersion = 0x04;
constexpr uint32_t kM01DSaveVersion = 0x07;

constexpr uint32_t GetSaveVersion(const SaveFormat saveFormat)
{
    switch (saveFormat)
    {
    case SaveFormat::M01: return kM01SaveVersion;
    case SaveFormat::M01D: return kM01DSaveVersion;
    }
    throw std::runtime_error("Unknown SaveFormat");
}

constexpr std::optional<SaveFormat> FormatFromSaveVersion(const uint32_t version)
{
    switch (version)
    {
    case kM01SaveVersion: return SaveFormat::M01;
    case kM01DSaveVersion: return SaveFormat::M01D;
    default: return std::nullopt;
    }
}

constexpr uint32_t kM01UninitializedSongLength = 0xA1F4;
constexpr uint32_t kM01DUninitializedSongLength = 0x300;

constexpr uint32_t GetUninitializedSongLength(const SaveFormat saveFormat)
{
    switch (saveFormat)
    {
    case SaveFormat::M01: return kM01UninitializedSongLength;
    case SaveFormat::M01D: return kM01DUninitializedSongLength;
    }
    throw std::runtime_error("Unknown SaveFormat");
}

constexpr uint32_t kM01DFileSizeRound = 0x100;


// clang-format off
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
// clang-format on

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
    std::vector<DrumInfo> drumInfos;
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

enum class Tag : uint16_t
{
    End = 0x0000,
    MasterInfo = 0x0201,
    MeasureInfo = 0x0104,
    PatternData = 0x0105,
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
    // Pattern stores *all* notes that are in the pattern, even ones that were
    // later truncated by shortening the measure's steps.
    // This must be cross-referenced with the measure's steps from its MeasureInfo
    // entry to ensure only active notes are used.
    std::vector<NoteData> notes;
};

struct Track
{
    Instrument instrument;
    std::vector<std::optional<Pattern>> measures;
};

constexpr uint32_t kSongIdentifierLength = 0x28;
struct SongIdentifier
{
    bool hasData;
    std::string name;
    uint32_t location; // Byte address for M01, file ID for M01D
    uint32_t length;
};

struct SongSource
{
    std::string name;
    std::optional<uint32_t> songLength;
};

enum class FXType : uint8_t
{
    Delay = 0,
    Reverb = 1,
};

struct SongData
{
    std::string name;
    std::string sourceSongName;
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

#endif // SAVESTRUCTURE_H
