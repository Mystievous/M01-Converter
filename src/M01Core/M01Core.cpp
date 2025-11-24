//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 11/23/2025
//

#include "M01Core/M01Core.h"
#include <stdexcept>
#include <filesystem>
#include <cstdio>

namespace M01Core
{

    std::vector<ConversionResult> ConvertSaveFile(const std::string &path, bool extended, const std::string &configPath)
    {
        if (!std::filesystem::exists(path))
        {
            throw std::runtime_error("Input file does not exist: " + path);
        }

        FILE *f = fopen(path.c_str(), "rb");
        if (!f)
        {
            throw std::runtime_error("Could not open file: " + path);
        }

        std::vector<ConversionResult> results = ConvertSaveFileFromFILE(f, extended, configPath);

        fclose(f);
        return results;
    }

    std::vector<ConversionResult> ConvertSaveFileFromFILE(FILE *file, bool extended, const std::string &configPath)
    {
        SaveFile save(file);
        if (!save.IsValid())
        {
            return {};
        }

        std::vector<ConversionResult> results;
        for (const auto &song : save.GetSongs())
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
