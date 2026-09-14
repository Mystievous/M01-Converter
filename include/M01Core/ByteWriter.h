//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 9/11/2026
//


#ifndef M01_CONVERTER_BYTEWRITER_H
#define M01_CONVERTER_BYTEWRITER_H

#include <bit>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <format>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <vector>
#include <span>
#include <algorithm>

static_assert(std::endian::native == std::endian::little, "M01 save encoding assumes a little-endian host");

class ByteWriter
{
public:
    [[nodiscard]] std::span<const std::byte> Bytes() const { return data_; }
    [[nodiscard]] std::vector<std::byte> Take() && { return std::move(data_); }
    [[nodiscard]] size_t Position() const { return data_.size(); }

    template <typename T>
        requires std::integral<T>
    void PatchAt(std::type_identity_t<T> value, const size_t position)
    {
        Require(position, sizeof(T));
        std::memcpy(data_.data() + position, &value, sizeof(T));
    }

    template <typename T>
        requires std::integral<T>
    void Write(std::type_identity_t<T> value)
    {
        const auto at = Position();
        data_.resize(at + sizeof(T));
        std::memcpy(data_.data() + at, &value, sizeof(T));
    }

    template <typename T>
        requires std::is_scoped_enum_v<T>
    void Write(const T value)
    {
        using U = std::underlying_type_t<T>;
        Write<U>(static_cast<U>(value));
    }

    void WriteBytes(std::span<const std::byte> bytes) { data_.insert(data_.end(), bytes.begin(), bytes.end()); }

    void WriteString(const std::string_view value, const size_t width)
    {
        const auto at = Position();
        // Anything past the string is 0x00
        data_.resize(at + width);
        // Truncate strings that are too long
        const auto length = std::min(value.size(), width);
        std::memcpy(data_.data() + at, value.data(), length);
    }

    void Pad(const std::byte value, const size_t count) { data_.insert(data_.end(), count, value); }

    void PadUntil(const std::byte value, const size_t untilAddress)
    {
        if (untilAddress < Position())
        {
            throw std::runtime_error(
                std::format("Tried to pad until 0x{:08X}, which is before the current position of 0x{:08X}",
                            untilAddress, Position()));
        }
        const auto length = untilAddress - Position();
        Pad(value, length);
    }

    [[nodiscard]] uint32_t SumBytes(const size_t length, const size_t offset) const
    {
        Require(offset, length);

        uint32_t sum = 0;
        for (const auto& byte : Bytes().subspan(offset, length))
        {
            sum += std::to_integer<uint8_t>(byte);
        }
        return sum;
    }

private:
    std::vector<std::byte> data_;

    void Require(size_t at, const size_t count) const
    {
        if (at > data_.size() || count > data_.size() - at)
        {
            throw std::out_of_range(
                std::format("Attempt to access {} bytes at offset {}, buffer is {} bytes", count, at, data_.size()));
        }
    }
};

#endif // M01_CONVERTER_BYTEWRITER_H
