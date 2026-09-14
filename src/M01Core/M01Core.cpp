//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 11/23/2025
//

#include "M01Core/M01Core.h"

#include <stdexcept>
#include <filesystem>

#include "M01Core/ByteReader.h"
#include "M01Core/FileBytes.h"
#include "M01Core/MidiExport.h"
#include "M01Core/SaveFile.h"
#include "M01Core/SongFile.h"
#include "M01Core/SaveStructure.h"
#include "M01Core/SaveEncoder.h"

namespace M01Core
{

    static std::vector<std::optional<SongData>> LoadSongs(const std::filesystem::path& path)
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

            return save.GetSongs();
        }
        else if (reader.Has(16, 0x1F0) && reader.ReadString(4, 0x1F0) == kSongMarker
                 && reader.ReadString(4, 0x1FC) == kSongMarker)
        {
            const auto songData = LoadSongFile(bytes, path);
            if (!songData.has_value())
            {
                throw std::runtime_error("Could not read file: " + path.string());
            }
            return {songData};
        }
        else
        {
            throw std::runtime_error("Unrecognized file format: " + path.string());
        }
    }

    std::vector<MIDIResult> ConvertToMidi(const std::filesystem::path& path, bool extended,
                                          const std::filesystem::path& configPath)
    {
        const auto songs = LoadSongs(path);
        std::vector<MIDIResult> results;
        for (const auto& song : songs)
        {
            if (!song.has_value())
                continue;
            MIDIResult r;
            r.name = song->name;
            r.midi = extended ? MakeExtendedMidiFile(*song, configPath) : MakeMidiFile(*song);

            results.push_back(std::move(r));
        }

        return results;
    }

    std::vector<EncodedSaveFile> ConvertToSave(const std::filesystem::path& path, SaveFormat format)
    {
        auto songs = LoadSongs(path);
        if (format == SaveFormat::M01)
        {
            if (songs.size() > kM01NumberOfSongs)
            {
                std::cout << std::format("{} songs exceeds the M01 maximum of {}.\n"
                                         "Songs will be packed together, then any extras will be dropped.\n",
                                         songs.size(), kM01NumberOfSongs);

                std::vector<std::optional<SongData>> songsM01;
                songsM01.reserve(kM01NumberOfSongs);

                for (size_t i = 0; i < songs.size(); i++)
                {
                    const auto& song = songs[i];

                    if (song.has_value())
                    {
                        if (songsM01.size() >= kM01NumberOfSongs)
                        {
                            std::cout << std::format("Song {} exceeds the maximum of {} songs, dropping...\n",
                                                     song->name, kM01NumberOfSongs);
                            continue;
                        }
                        songsM01.emplace_back(song);
                    }
                }

                songs = std::move(songsM01);
            }
            songs.resize(kM01NumberOfSongs);
        }

        return EncodeSave(songs, format);
    }
} // namespace M01Core
