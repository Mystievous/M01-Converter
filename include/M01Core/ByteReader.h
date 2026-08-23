//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 8/22/2026
//

#ifndef M01_CONVERTER_BYTEREADER_H
#define M01_CONVERTER_BYTEREADER_H

#include <bit>
#include <concepts>
#include <cstddef>
#include <cstring>
#include <format>
#include <span>
#include <stdexcept>
#include <cstdint>

static_assert(std::endian::native == std::endian::little,
              "M01 save decoding assumes a little-endian host");

class ByteReader
{
public:
    explicit ByteReader(const std::span<const std::byte> data,
                        const size_t pos = 0)
        : data_(data), pos_(pos)
    {
    }

    [[nodiscard]] size_t Position() const { return pos_; }
    [[nodiscard]] size_t Remaining() const { return data_.size() - pos_; }

    template <typename T>
        requires std::integral<T>
    T Read()
    {
        Require(pos_, sizeof(T));
        T value{};
        std::memcpy(&value, data_.data() + pos_, sizeof(T));
        pos_ += sizeof(T);
        return value;
    }

    void Skip(const size_t count)
    {
        Require(pos_, count);
        pos_ += count;
    }

    void Seek(const size_t pos)
    {
        Require(pos, 0);
        pos_ = pos;
    }

    std::span<const std::byte> ReadBytes(const size_t count)
    {
        Require(pos_, count);
        const auto bytes = data_.subspan(pos_, count);
        pos_ += count;
        return bytes;
    }

    [[nodiscard]] uint32_t SumBytes(const size_t offset, const size_t length) const
    {
        Require(offset, length);

        uint32_t sum = 0;
        for (const auto& byte : data_.subspan(offset, length))
        {
            sum += std::to_integer<uint8_t>(byte);
        }
        return sum;
    }

private:
    std::span<const std::byte> data_;
    size_t pos_;

    void Require(size_t at, const size_t count) const
    {
        if (at > data_.size() || count > data_.size() - at)
        {
            throw std::out_of_range(std::format(
                "Attempt to read {} bytes at offset {}, buffer is {} bytes", count,
                at, data_.size()));
        }
    }
};

#endif // M01_CONVERTER_BYTEREADER_H
