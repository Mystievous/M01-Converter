//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 8/27/2026
//

#include "M01Core/SongDecoder.h"

#include <iostream>

enum class Tag : uint16_t
{
    End = 0x0000,
    MasterInfo = 0x0201,
    MeasureInfo = 0x0104,
    PatternData = 0x0105,
};

void DecodeSongData(ByteReader& reader, const SongIdentifier& identifier)
{
    reader.Seek(identifier.songStartAddress);
    const auto songChecksum = reader.Read<uint32_t>();

    const auto calculatedChecksum = reader.SumBytes(identifier.songStartAddress + 0x04,
                                                    identifier.songLength - 0x04);
    if (songChecksum != calculatedChecksum)
    {
        std::cerr << std::hex << std::uppercase << "Song checksum mismatch for " << identifier.name <<
            ". Expected: `\\x" <<
            songChecksum << "`, Calculated: `\\x" << calculatedChecksum << "`." << std::dec << std::endl;
    }

    const auto songVersion = reader.Read<uint32_t>();
    reader.Skip(0x1E8);
    if (const auto label = reader.ReadString(4); label != "song")
    {
        std::cerr << "Song label is not `song`, instead: " << label << std::endl;
    }

    const auto songDataStart = reader.Position();
    const auto songDataLength = reader.Read<uint32_t>(); // Length includes its own bytes
    const auto songDataVersion = reader.Read<uint32_t>();

    const auto songEndAddress = identifier.songStartAddress + identifier.songLength;

    if (songEndAddress != songDataStart + songDataLength)
    {
        std::cerr << std::hex << std::uppercase << "Mismatch in song length values. File Header: " << songEndAddress <<
            ", Song Data: " << songDataStart + songDataLength << std::dec << std::endl;
    }

    reader.Skip(0x04);

    while (reader.Position() < songEndAddress)
    {
        const auto tag = static_cast<Tag>(reader.Read<uint16_t>());
        const auto length = reader.Read<uint16_t>();
        const auto payloadStart = reader.Position();

        reader.Skip(length);

        std::cout << std::hex << std::uppercase << "Tag: " << static_cast<uint16_t>(tag) << std::dec << std::endl;
        std::cout << std::hex << std::uppercase << "Payload Start: " << payloadStart - 0x04 << std::dec << std::endl;
        std::cout << std::hex << std::uppercase << "Length: " << length << std::dec << std::endl;

        if (reader.Position() != payloadStart + length)
        {
            std::cerr << std::hex << std::uppercase << "Payload does not end at the correct address. Expected: " <<
                payloadStart + length
                <<
                ", Found: " << reader.Position() << std::dec << std::endl;
        }

        if (tag == Tag::End) break;

        std::cout << std::endl;
    }
    if (reader.Position() != songEndAddress)
    {
        std::cerr << std::hex << std::uppercase << "Song does not end at the correct address. Expected: " <<
            songEndAddress <<
            ", Found: " << reader.Position() << std::dec << std::endl;
    }
}
