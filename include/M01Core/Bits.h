//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 8/22/2026
//

#ifndef M01_CONVERTER_BITS_H
#define M01_CONVERTER_BITS_H

#include <type_traits>

namespace bits
{
    template <unsigned Offset, unsigned Width, typename T>
    [[nodiscard]] constexpr auto Get(T value)
    {
        static_assert(Offset + Width <= sizeof(T) * 8, "bit range exceeds the width of T");
        auto field = value >> Offset;
        field = field & ((1 << Width) - 1);
        // Boolean if only one bit, otherwise the value.
        if constexpr (Width == 1)
        {
            return field != 0;
        }
        else
        {
            return static_cast<T>(field);
        }
    }

    template <unsigned Offset, unsigned Width, typename T>
    [[nodiscard]] constexpr T Set(const std::conditional_t<Width == 1, bool, T> value, T container)
    {
        static_assert(Offset + Width <= sizeof(T) * 8, "bit range exceeds the width of T");
        // Width=5, 00100000 -> 00011111; Subtracting 1 fills everything below it
        constexpr T mask = static_cast<T>((T{1} << Width) - 1);
        // Offset mask to the correct offset (where we'll write the value), invert it, and do an & operation
        // Everything else stays but the area we're writing into is reset/cleared.
        container &= static_cast<T>(~(mask << Offset));
        // Offset the new value (masked to the specified Width) and write it into the cleared area.
        container |= static_cast<T>((value & mask) << Offset);
        return container;
    }

} // namespace bits

#endif // M01_CONVERTER_BITS_H
