//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 8/28/2026
//

#ifndef M01_CONVERTER_MIDIEXPORT_H
#define M01_CONVERTER_MIDIEXPORT_H

#include "M01Core/SaveStructure.h"
#include "MidiFile.h"

smf::MidiFile MakeMidiFile(const SongData& song);

smf::MidiFile MakeExtendedMidiFile(const SongData& song, const std::string& configPath);

#endif //M01_CONVERTER_MIDIEXPORT_H
