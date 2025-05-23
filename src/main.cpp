//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/15/2025
//

#include <filesystem>
#include <iostream>
#include <SaveFile.h>

#include "SaveStructure.h"
#include "Song.h"
#include "MidiFile.h"
#include "cxxopts.hpp"

constexpr std::string kFileSignature = "M01W";

int main(const int argc, char** argv)
{
    cxxopts::Options options("M01-Converter", "Convert Korg M01 DS songs to MIDI files");
    options.add_options()
        ("h,help", "Show help")
        ("e,extended", "Create an 'extended' MIDI file.")
        ("c,config", "Path to a custom config file for 'extended' export. By default it is 'config.yml'.",
         cxxopts::value<std::string>())
        ("i,input", "Input .sav file", cxxopts::value<std::string>());

    options.parse_positional({"input"});
    options.positional_help("input_sav");

    const auto result = options.parse(argc, argv);

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
        if (!std::filesystem::exists(inputPath))
        {
            std::cerr << "Error: File does not exist." << std::endl;
            return 1;
        }
        FILE* saveFile = fopen(inputPath.c_str(), "rb");
        std::cout << "Opened file: " << inputPath << std::endl;
        const SaveFile saveData(saveFile);
        if (!saveData.IsValid())
        {
            std::cerr << "Error: Invalid save file." << std::endl;
            fclose(saveFile);
            return 1;
        }
        std::cout << "Found " << saveData.GetNumberOfSongs() << " songs." << std::endl;
        fclose(saveFile);

        if (result.count("extended"))
        {
            for (const auto& song : saveData.GetSongs())
            {
                std::cout << "Making extended MIDI file for: " << song.identifier.name << ".mid" << std::endl;
                if (result.count("config"))
                {
                    auto& midiFile = song.MakeExtendedMidiFile(result["config"].as<std::string>());
                    std::string filename = song.identifier.name;
                    midiFile.write(filename + ".mid");
                }
                else
                {
                    auto& midiFile = song.MakeExtendedMidiFile();
                    std::string filename = song.identifier.name;
                    midiFile.write(filename + ".mid");
                }
            }
        }
        else
        {
            for (const auto& song : saveData.GetSongs())
            {
                std::cout << "Making MIDI file for: " << song.identifier.name << ".mid" << std::endl;
                auto& midiFile = song.MakeMidiFile();
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
