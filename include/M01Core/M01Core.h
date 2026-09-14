//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 11/23/2025
//
#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "MidiFile.h"

#include "M01Core/SaveEncoder.h"

namespace M01Core
{
    // High-level conversion result for a single song
    struct MIDIResult
    {
        std::string name;
        smf::MidiFile midi;
    };

    std::vector<MIDIResult> ConvertToMidi(const std::filesystem::path& path, bool extended,
                                          const std::filesystem::path& configPath);

    std::vector<EncodedSaveFile> ConvertToSave(const std::filesystem::path& path, SaveFormat format);
} // namespace M01Core

enum class ExportType
{
    MIDI,
    M01,
    M01D
};
