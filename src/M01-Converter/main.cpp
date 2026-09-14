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
#include "M01Core/FileBytes.h"
#include "cxxopts.hpp"
#include "M01Core/M01Core.h"

#include "M01Core/SaveEncoder.h"


constexpr std::filesystem::path MakeOutputPath(const std::filesystem::path& outputDir, const std::string_view filename)
{
    auto outputPath = outputDir / filename;
    const auto stem = outputPath.stem();
    const auto extension = outputPath.extension();
    auto count = 0;
    while (std::filesystem::exists(outputPath))
    {
        count += 1;
        outputPath = outputDir / std::format("{} ({}){}", stem.string(), count, extension.string());
    }
    return outputPath;
}

constexpr std::string SanitizeFileName(std::string_view name)
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

int main(const int argc, char** argv)
{
    cxxopts::Options options("M01-Converter", "Convert save files from Korg M01 (NDS) and M01D (3DS) software.");
    // clang-format off
    options.add_options()
        ("h,help", "Show help")
        ("e,extended", "Create an 'extended' MIDI file. Only applies to MIDI export type.")
        ("c,config", "Path to a custom config file for 'extended' export. By default it is 'config.yml'.", cxxopts::value<std::string>())
        ("t,type", "The type of output, options are `MIDI`, `M01`, and `M01D`", cxxopts::value<std::string>())
        ("o,output", "Directory to output files into.", cxxopts::value<std::string>())
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
        const auto type = result.count("type") ? result["type"].as<std::string>() : "MIDI";

        std::cout << std::format("Converting {} to {} format...\n", inputPath.string(), type);
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

        if (type == "MIDI")
        {
            auto results = M01Core::ConvertToMidi(inputPath, extendedMode, configPath);
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
        else if (type == "M01" || type == "NDS")
        {
            const auto files = M01Core::ConvertToSave(inputPath, SaveFormat::M01);
            if (files.size() != 1)
            {
                throw std::runtime_error("M01 save converted to more than one file. This should never be reached.");
            }
            const auto& file = files[0];

            const auto outputPath = MakeOutputPath(outputDir, file.filename);
            WriteWholeFile(outputPath, file.bytes);
        }
        else if (type == "M01D" || type == "3DS")
        {
            const auto files = M01Core::ConvertToSave(inputPath, SaveFormat::M01D);
            for (const auto& file : files)
            {
                const auto outputPath = outputDir / file.filename;
                WriteWholeFile(outputPath, file.bytes);
            }
        }
        else
        {
            throw std::invalid_argument(
                std::format("Invalid export type: {}. Options are `MIDI`, `M01`, and `M01D`", type));
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
