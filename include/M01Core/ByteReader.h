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
#include <string>
#include <string_view>

static_assert(std::endian::native == std::endian::little, "M01 save decoding assumes a little-endian host");

class ByteReader
{
public:
    explicit ByteReader(const std::span<const std::byte> data, const size_t pos = 0) : data_(data), pos_(pos) {}

    [[nodiscard]] size_t Position() const { return pos_; }
    [[nodiscard]] size_t Remaining() const { return data_.size() - pos_; }
    [[nodiscard]] size_t Size() const { return data_.size(); }
    [[nodiscard]] bool Has(const size_t length, const size_t offset) const
    {
        return offset <= data_.size() && length <= data_.size() - offset;
    }

    template <typename T>
        requires std::integral<T>
    T Read()
    {
        const auto value = Read<T>(pos_);
        pos_ += sizeof(T);
        return value;
    }

    template <typename T>
        requires std::integral<T>
    [[nodiscard]] T Read(const size_t position) const
    {
        Require(position, sizeof(T));
        T value{};
        std::memcpy(&value, data_.data() + position, sizeof(T));
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
        const auto value = ReadBytes(count, pos_);
        pos_ += count;
        return value;
    }

    [[nodiscard]] std::span<const std::byte> ReadBytes(const size_t count, const size_t position) const
    {
        Require(position, count);
        const auto bytes = data_.subspan(position, count);
        return bytes;
    }

    std::string ReadString(const size_t width)
    {
        const auto bytes = ReadBytes(width);
        const std::string_view view(reinterpret_cast<const char*>(bytes.data()), width);
        return std::string(view.substr(0, view.find('\0')));
    }

    [[nodiscard]] std::string ReadString(const size_t width, const size_t position) const
    {
        const auto bytes = ReadBytes(width, position);
        const std::string_view view(reinterpret_cast<const char*>(bytes.data()), width);
        return std::string(view.substr(0, view.find('\0')));
    }

    [[nodiscard]] uint32_t SumBytes(const size_t length, const size_t offset) const
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
            throw std::out_of_range(
                std::format("Attempt to read {} bytes at offset {}, buffer is {} bytes", count, at, data_.size()));
        }
    }
};

#endif // M01_CONVERTER_BYTEREADER_H
