//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/15/2025
//

#include "M01Core/Measure.h"

Measure::Measure(FILE *saveFile) : trackHeaders{}, noteData{}
{
    for (int i = 0; i < kNumberOfInstruments; ++i)
    {
        fread(&trackHeaders[i], sizeof(TrackHeader), 1, saveFile);
        noteData[i].resize(trackHeaders[i].numberOfNotes);
        fread(noteData[i].data(), sizeof(NoteData), trackHeaders[i].numberOfNotes, saveFile);
    }
}
