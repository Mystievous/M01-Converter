//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/21/2025
//

#include "M01Core/SaveFile.h"

#include <string>
#include <string_view>

#include "FileBytes.h"

constexpr std::string_view kFileSignature = "M01W";
constexpr uint32_t kDSVersions[] = {
    0x04
};

static bool check_header_version(const uint32_t version)
{
    const auto foundVersion = std::ranges::find(kDSVersions, version);
    return foundVersion != std::end(kDSVersions);
}

SaveFile::SaveFile(FILE *saveFile)
{
    FileHeader header{};
    fread(&header, sizeof(FileHeader), 1, saveFile);

    // Sanity check, is this a save for the right app.
    isValid = std::string(header.signature, 4) == kFileSignature;

    // Checks the header's checksum. The first four bytes of the save file (u32) should be the value of all other bytes
    // in the header area added together/summed.
    if (const auto sum = sum_bytes(header.signature) + sum_bytes(header.version) + sum_bytes(header.songIdentifiers); header.checksum != sum)
    {
        std::cerr << std::hex << std::uppercase << "Header checksum mismatch. Expected: `\\x" << header.checksum << "`. Calculated: `\\x" << sum << "`." << std::endl;
        isValid = false;
    }

    if (!check_header_version(header.version))
    {
        std::cerr << "WARNING: Save file format version " << header.version << " is not officially supported by this tool. Trying to parse anyways." << std::endl;
    }

    // Only continue if the file has the proper signature, and a valid checksum.
    if (isValid)
    {
        // Parse each stored song one at a time.
        for (const auto &songIdentifier : header.songIdentifiers)
        {
            if (songIdentifier.songHasData)
            {
                songs.emplace_back(saveFile, songIdentifier);
            }
        }
    }
}

bool SaveFile::IsValid() const
{
    return isValid;
}
