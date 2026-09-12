//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/15/2025
//

#include <cctype>
#include <string_view>
#include <string>
#include <filesystem>
#include <iostream>
#include <format>
#include "cxxopts.hpp"
#include "M01Core/M01Core.h"

namespace
{
    std::string SanitizeFileName(std::string_view name)
    {
        std::string out;
        out.reserve(name.size());
        for (const unsigned char c : name)
        {
            const bool keep = std::isalnum(c) != 0 || c == ' ' || c == '_' || c == '-' || c == '(' || c == ')';
            out.push_back(keep ? static_cast<char>(c) : '_');
        }

        // Strip whitespace at the end
        while (!out.empty() && out.back() == ' ')
        {
            out.pop_back();
        }

        // If nothing left
        if (out.empty())
        {
            out = "song";
        }

        return out;
    }
} // namespace

int main(const int argc, char** argv)
{
    cxxopts::Options options("M01-Converter", "Convert songs from Korg M01 (NDS) and M01D (3DS) to MIDI files");
    // clang-format off
    options.add_options()
        ("h,help", "Show help")
        ("e,extended", "Create an 'extended' MIDI file.")
        ("c,config", "Path to a custom config file for 'extended' export. By default it is 'config.yml'.", cxxopts::value<std::string>())
        ("o,output", "Directory to output MIDI files into.", cxxopts::value<std::string>())
        ("i,input", "Input file", cxxopts::value<std::string>());
    // clang-format on

    options.parse_positional({"input"});
    options.positional_help("input_file");

    const auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        return 0;
    }

    std::filesystem::path inputPath;

    if (result.count("input"))
    {
        inputPath = result["input"].as<std::string>();
    }
    else
    {
        std::cout << options.help() << std::endl;
        return 0;
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
        const std::filesystem::path configPath =
            result.count("config") ? result["config"].as<std::string>() : "config.yml";

        if (extendedMode && !std::filesystem::exists(configPath))
        {
            std::cerr << std::format("Error: Config file not found: {}\n", configPath.string());
            return 1;
        }

        const std::filesystem::path outputDir = result.count("output") ? result["output"].as<std::string>() : ".";
        if (!std::filesystem::exists(outputDir))
        {
            std::filesystem::create_directories(outputDir);
        }
        if (!std::filesystem::is_directory(outputDir))
        {
            std::cerr << std::format("ERROR: Output directory is not a directory: {}\n", outputDir.string());
            return 1;
        }

        auto results = M01Core::ConvertFile(inputPath, extendedMode, configPath);
        std::cout << std::format("Found {} song{}.\n", results.size(), results.size() == 1 ? "" : "s");

        for (auto& r : results)
        {
            auto filename = SanitizeFileName(r.name);

            auto outputPath = outputDir / std::format("{}.mid", filename);
            auto count = 0;

            while (std::filesystem::exists(outputPath))
            {
                count += 1;
                outputPath = outputDir / std::format("{} ({}).mid", filename, count);
            }

            const auto type = extendedMode ? "extended MIDI" : "MIDI";
            std::cout << std::format("Making {} file: {}\n", type, outputPath.filename().string());

            r.midi.write(outputPath.string());
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
