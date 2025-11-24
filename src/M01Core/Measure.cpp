//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/15/2025
//

#include <stdexcept>
#include <cerrno>
#include <cstring>

#include "M01Core/Measure.h"

Measure::Measure(FILE *saveFile) : trackHeaders{}, noteData{}
{
    for (int i = 0; i < kNumberOfInstruments; ++i)
    {
        size_t trackHeadersReadCount = fread(&trackHeaders[i], sizeof(TrackHeader), 1, saveFile);
        if (trackHeadersReadCount != 1)
        {
            if (feof(saveFile))
                throw std::runtime_error("Unexpected EOF while reading TrackHeader");
            else
                throw std::runtime_error(std::string("fread failed: ") + std::strerror(errno));
        }
        noteData[i].resize(trackHeaders[i].numberOfNotes);
        size_t noteDataReadCount = fread(noteData[i].data(), sizeof(NoteData), trackHeaders[i].numberOfNotes, saveFile);
        if (noteDataReadCount != trackHeaders[i].numberOfNotes)
        {
            if (feof(saveFile))
                throw std::runtime_error("Unexpected EOF while reading NoteData");
            else
                throw std::runtime_error(std::string("fread failed: ") + std::strerror(errno));
        }
    }
}
