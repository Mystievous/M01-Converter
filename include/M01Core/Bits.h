//
// Created by myst on 8/22/26.
//

#ifndef M01_CONVERTER_BITS_H
#define M01_CONVERTER_BITS_H

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
}

#endif //M01_CONVERTER_BITS_H
