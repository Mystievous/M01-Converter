//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/15/2025
//

#ifndef SONG_H
#define SONG_H

#include "InstrumentHelper.h"
#include "SaveStructure.h"
#include "Measure.h"
#include "MidiFile.h"

struct InstrumentPlaybackInfo
{
    uint8_t track = 0;
    uint8_t channel = 0;
    uint8_t bank = 0;
    uint8_t subBank = 0;
    uint8_t program = 0;
};

class Song
{
public:
    InstrumentHelper instrumentHelper;

    SongIdentifier identifier;
    SongHeader header;
    std::vector<Measure> measures;

    Song(FILE* saveFile, const SongIdentifier& identifier);

    [[nodiscard]] int ticksToSwing(int ticks) const;
    [[nodiscard]] smf::MidiFile& makeMidiFile() const;
    [[nodiscard]] smf::MidiFile& makeExtendedMidiFile(const std::string& configPath = "config.yml") const;
};


#endif //SONG_H
