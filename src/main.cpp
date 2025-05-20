//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/15/2025
//

#include <iostream>

#include "SaveStructure.h"
#include "Song.h"
#include "MidiFile.h"
#include "cxxopts.hpp"
#include "yaml-cpp/yaml.h"

constexpr std::string kFileSignature = "M01W";

int main(int argc, char** argv)
{
    cxxopts::Options options("M01D-Converter", "Convert Korg M01D DS songs to MIDI files");
    options.add_options()
        ("h,help", "Show help")
        ("e,extended", "Create an 'extended' MIDI file.")
        ("c,config", "Path to a custom config file for 'extended' export. By default it is 'config.yml'.", cxxopts::value<std::string>())
        ("i,input", "Input .sav file", cxxopts::value<std::string>());

    options.parse_positional({"input"});
    options.positional_help("input_sav");

    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        return 1;
    }

    std::string inputPath;

    if (result.count("input"))
    {
        inputPath = result["input"].as<std::string>();
    }
    else
    {
        std::cout << "Path to .sav file:" << std::endl;
        std::cout << "> ";
        std::getline(std::cin, inputPath);
    }


    try
    {
        std::cout << "Converting " << inputPath << " to MIDI files..." << std::endl;
        FILE* saveFile = fopen(inputPath.c_str(), "rb");
        FileHeader& header = *new FileHeader();
        fread(&header, sizeof(FileHeader), 1, saveFile);
        if (auto signature = std::string(header.signature, 4); signature != kFileSignature)
        {
            std::cerr << "Error: File does not seem to be a valid M01D DS .sav File." << std::endl;
            fclose(saveFile);
            return 1;
        }
        std::vector<Song> songs;
        for (const auto& songIdentifier : header.songIdentifiers)
        {
            if (songIdentifier.songHasData)
            {
                auto songData = Song(saveFile, songIdentifier);
                songs.push_back(songData);
            }
        }
        std::cout << "Found " << songs.size() << " songs." << std::endl;
        fclose(saveFile);

        if (result.count("extended"))
        {
            for (const auto& song : songs)
            {
                std::cout << "Making extended MIDI file for: " << song.identifier.name << ".mid" << std::endl;
                if (result.count("config"))
                {
                    auto& midiFile = song.makeExtendedMidiFile(result["config"].as<std::string>());
                    std::string filename = song.identifier.name;
                    midiFile.write(filename + "_gen.mid");
                }
                else
                {
                    auto& midiFile = song.makeExtendedMidiFile();
                    std::string filename = song.identifier.name;
                    midiFile.write(filename + "_gen.mid");
                }
            }
        } else
        {
            for (const auto& song : songs)
            {
                std::cout << "Making MIDI file for: " << song.identifier.name << ".mid" << std::endl;
                auto& midiFile = song.makeMidiFile();
                std::string filename = song.identifier.name;
                midiFile.write(filename + ".mid");
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
