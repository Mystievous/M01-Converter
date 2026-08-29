//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/15/2025
//

#ifndef SONG_H
#define SONG_H

#include "SaveStructure.h"
#include "Measure.h"
#include "MidiFile.h"

class Song
{
public:
    SongIdentifier identifier;
    SongHeader header;
    std::vector<Measure> measures;

    [[nodiscard]] smf::MidiFile& MakeMidiFile() const;
    [[nodiscard]] smf::MidiFile& MakeExtendedMidiFile(const std::string& configPath = "config.yml") const;
};


#endif //SONG_H
