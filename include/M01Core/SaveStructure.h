//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/15/2025
//

#ifndef SAVESTRUCTURE_H
#define SAVESTRUCTURE_H

#include <cstdint>
#include <string>

constexpr int kNumberOfSongs = 10;
constexpr int kNumberOfInstruments = 8;
constexpr int kNumberOfMeasures = 99;

struct PlaybackFlags
{
    bool hasFX = false;
    bool muted = false;
    bool soloed = false;
    uint8_t kaosKey = 0;
};

struct DrumInfo
{
    uint8_t level;
    int8_t timestretch;
    int8_t panning;
};

struct Instrument
{
    uint8_t _1[0x08];
    uint8_t bank;
    uint8_t subBank;
    uint8_t program;
    uint8_t _2[0x02];
    uint8_t attack;
    uint8_t release;
    uint8_t volume;
    int8_t panning;
    PlaybackFlags playbackState;
    uint8_t _3[0x05];
    DrumInfo drumInfo[8];
    uint8_t _4[0x09];
} __attribute__((packed));

struct PlaybackInfo
{
    uint8_t hasSolo : 1;
} __attribute__((packed));

struct ReverbInfo
{
    uint8_t reverbType;
    uint8_t reverbTime;
    uint8_t reverbLevel;
} __attribute__((packed));

struct DelayInfo
{
    bool syncOn;
    uint8_t time;
    int8_t panRatio;
    uint8_t feedback;
    uint8_t level;
    uint8_t _1[0x03];
} __attribute__((packed));

struct SceneState
{
    bool reverbOn = false;
    bool sceneLocked = false;
    uint8_t swing = 0;
};

struct MasterInfo
{
    uint16_t tempo;
    uint8_t swing;
    uint8_t step;
} __attribute__((packed));

struct MeasureInfo
{
    uint16_t tempo;
    uint8_t step;
    uint8_t _1[0x05];
} __attribute__((packed));

struct NoteData
{
    uint8_t length;
    uint8_t velocity;
    uint8_t noteId;
    uint8_t startPoint;
} __attribute__((packed));

struct BlockInfo
{
    uint8_t _1[0x02];
    uint8_t blockLength;
    uint8_t _2[0x01];
} __attribute__((packed));

struct TrackHeader
{
    BlockInfo blockInfo;
    uint8_t measureNumber;
    uint8_t trackNumber;
    uint8_t numberOfNotes;
    uint8_t _1[0x01];
} __attribute__((packed));

struct SongHeader
{
    Instrument instruments[kNumberOfInstruments];
    uint8_t _1[0x08];
    PlaybackInfo playbackInfo;
    uint8_t _2[0x03];
    ReverbInfo reverbInfo;
    uint8_t _3[0x01];
    DelayInfo delayInfo;
    SceneState sceneState;
    uint8_t _4[0x2D];
    MasterInfo masterInfo;
    uint8_t _5[0x06];
    MeasureInfo measureInfo[kNumberOfMeasures];
} __attribute__((packed));

struct SongIdentifier
{
    bool songHasData;
    std::string name;
    uint32_t songStartAddress;
    uint32_t songLength;
};

struct SaveHeader
{
    const uint32_t checksum;
    const std::string signature; // "M01W"
    const uint32_t version;
};

#endif //SAVESTRUCTURE_H
