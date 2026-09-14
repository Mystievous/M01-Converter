//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 9/11/2026
//

#ifndef SONGFILE_H
#define SONGFILE_H

#include <filesystem>
#include <optional>
#include <span>
#include <cstddef>

#include "M01Core/SaveStructure.h"

std::optional<SongData> LoadSongFile(std::span<const std::byte> bytes, const std::filesystem::path& path);

#endif // SONGFILE_H
