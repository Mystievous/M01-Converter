//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 11/23/2025
//

#include "M01Core/M01Core.h"
#include <stdexcept>
#include <cstddef>

#include "M01Core/FileBytes.h"

namespace M01Core
{
    std::vector<ConversionResult> ConvertSaveFile(const std::string& path, bool extended, const std::string& configPath)
    {
        const std::vector<std::byte> bytes = ReadWholeFile(path);
        const SaveFile save(bytes);

        if (!save.IsValid())
        {
            throw std::runtime_error("Invalid save file: " + path);
        }

        std::vector<ConversionResult> results;
        for (const auto& song : save.GetSongs())
        {
            ConversionResult r;
            r.name = song.identifier.name;
            if (extended)
            {
                r.midi = &song.MakeExtendedMidiFile(configPath);
            }
            else
            {
                r.midi = &song.MakeMidiFile();
            }
            results.push_back(r);
        }

        return results;
    }
} // namespace M01Core
