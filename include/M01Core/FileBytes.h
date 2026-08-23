//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 8/21/2026
//

#ifndef M01_CONVERTER_FILEBYTES_H
#define M01_CONVERTER_FILEBYTES_H
#include <cstddef>
#include <numeric>
#include <string>
#include <vector>


template <typename T>
size_t sum_bytes(const T& obj)
{
    const auto p = reinterpret_cast<const unsigned char*>(&obj);
    return std::accumulate(p, p + sizeof(T), 0u);
}

std::vector<std::byte> ReadWholeFile(const std::string& path);

#endif //M01_CONVERTER_FILEBYTES_H
