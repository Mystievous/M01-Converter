//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 11/23/2025
//
//
#include "M01Core/M01Core.h"

#include <stdexcept>
#include <filesystem>
#include <iostream>

#include "M01Core/ByteReader.h"
#include "M01Core/FileBytes.h"
#include "M01Core/MidiExport.h"
#include "M01Core/SaveFile.h"
#include "M01Core/SongFile.h"
#include "M01Core/SaveStructure.h"

namespace M01Core
{
    std::vector<ConversionResult> ConvertFile(const std::filesystem::path& path, const bool extended,
                                              const std::filesystem::path& configPath)
    {
        const std::vector<std::byte> bytes = ReadWholeFile(path);

        // Detection of what filetype it is
        ByteReader reader(bytes);
        if (reader.Has(12, 0x00) && reader.ReadString(4, 0x04) == kFileSignature)
        {
            const SaveFile save(bytes, path.parent_path());

            if (!save.IsValid())
            {
                throw std::runtime_error("Invalid save file: " + path.string());
            }

            std::vector<ConversionResult> results;
            for (const auto& song : save.GetSongs())
            {
                ConversionResult r;
                r.name = song.name;
                r.midi = extended ? MakeExtendedMidiFile(song, configPath) : MakeMidiFile(song);

                results.push_back(std::move(r));
            }

            return results;
        }
        else if (reader.Has(16, 0x1F0) && reader.ReadString(4, 0x1F0) == kSongMarker
                 && reader.ReadString(4, 0x1FC) == kSongMarker)
        {
            const auto songData = LoadSongFile(bytes, path);
            if (!songData.has_value())
            {
                std::cerr << "Error reading file." << std::endl;
                return {};
            }
            ConversionResult result{
                .name = songData->name,
                .midi = extended ? MakeExtendedMidiFile(*songData, configPath) : MakeMidiFile(*songData),
            };
            return {std::move(result)};
        }
        else
        {
            throw std::runtime_error("Unrecognized file format: " + path.string());
        }
    }
} // namespace M01Core
