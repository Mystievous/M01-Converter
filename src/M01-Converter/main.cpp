//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/15/2025
//

#include <filesystem>
#include <iostream>
#include "cxxopts.hpp"
#include "M01Core/M01Core.h"

int main(const int argc, char** argv)
{
    cxxopts::Options options("M01-Converter", "Convert Korg M01 DS songs to MIDI files");
    options.add_options()("h,help", "Show help")("e,extended", "Create an 'extended' MIDI file.")(
        "c,config", "Path to a custom config file for 'extended' export. By default it is 'config.yml'.",
        cxxopts::value<std::string>())("i,input", "Input .sav file", cxxopts::value<std::string>());

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
        // Use the M01Core library API
        const bool extendedMode = result.count("extended") != 0;
        const std::string configPath = result.count("config")
                                           ? result["config"].as<std::string>()
                                           : std::string("config.yml");

        auto results = M01Core::ConvertSaveFile(inputPath, extendedMode, configPath);
        std::cout << "Found " << results.size() << " songs." << std::endl;

        for (auto& r : results)
        {
            if (extendedMode)
            {
                std::cout << "Making extended MIDI file for: " << r.name << ".mid" << std::endl;
            }
            else
            {
                std::cout << "Making MIDI file for: " << r.name << ".mid" << std::endl;
            }
            r.midi.write(r.name + ".mid");
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
