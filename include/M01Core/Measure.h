//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/15/2025
//

#ifndef MEASURE_H
#define MEASURE_H

#include <vector>
#include <iostream>

#include "SaveStructure.h"


class Measure
{
public:
    TrackHeader trackHeaders[kNumberOfInstruments];
    std::vector<NoteData> noteData[kNumberOfInstruments];

    explicit Measure(FILE* saveFile);
};


#endif //MEASURE_H
