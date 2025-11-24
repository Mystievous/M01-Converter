#pragma once

#include <string>
#include <vector>

#include "SaveFile.h"
#include "MidiFile.h"

namespace M01Core
{

    // High-level conversion result for a single song
    struct ConversionResult
    {
        std::string name;    // suggested filename (without extension)
        smf::MidiFile *midi; // ownership: caller is responsible for deleting the pointer
    };

    // Convert a .sav file at `path` and return conversion results for each song.
    // Throws std::runtime_error on I/O/format errors.
    std::vector<ConversionResult> ConvertSaveFile(const std::string &path, bool extended = false, const std::string &configPath = "config.yml");

} // namespace M01Core
