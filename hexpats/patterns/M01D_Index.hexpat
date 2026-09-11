#pragma description Korg M01D (3DS) save index
#pragma magic [ 4D 30 31 57 07 00 00 00 ] @ 0x04

#include <std/sys.pat>
#include <std/io.pat>

bool VALIDATE_CHECKSUMS = true;
u32 M01D_INDEX_VERSION = 0x07;

fn sum_range(u128 offsetFrom, u128 offsetTo) {
    u32 acc = 0;
    for (u128 i = offsetFrom, i < offsetTo, i = i + 1) {
        acc = (acc + std::mem::read_unsigned(i, 1)) & 0xFFFFFFFF;
    }
    return acc;
};

fn format_file_id(u32 id) {
    return std::format("M01Dn_{:08x}", id);
};
using SongFileId = u32 [[format("format_file_id")]];

struct SongIdentifier {
    // Both "padding" fields contain junk data for any songs past 10.
    bool songHasData [[color("00ff00")]];
    char name[8] [[color("ff00ff")]];
    u8 padding01[0x0F] [[color("404040")]];
    SongFileId songFileId [[color("00ffff")]];
    u32 songLength [[color("0091ff")]];
    u8 padding02[0x08] [[color("404040")]];
};

fn check_header_sum(u32 stored) {
    u32 computed = 0;
    bool valid = false;
    if (VALIDATE_CHECKSUMS) {
        // Sum of all bytes from immediately after the checksum u32 until
        // 0x04D (exclusive), which is where the song identifiers start.
        computed = sum_range(0x04, 0x04D);
        valid = stored == computed;
        std::assert_warn(valid, "header checksum mismatch");
    }
    return std::format("{:08X} {}", stored, valid ? "(OK)" : (VALIDATE_CHECKSUMS ? std::format("(BAD, want {:08X})", computed) : ""));
};
using HeaderChecksum = u32 [[format("check_header_sum")]];

struct Header {
    if (VALIDATE_CHECKSUMS) { 
        HeaderChecksum checksum [[color("B52D1B")]];
    } else {
        u32 checksum [[color("871E10")]];
    }
    char signature[4] [[color("0091ff")]];
    std::assert(signature == "M01W", "Not a Korg M01 save file");
    // always `7` for M01D/3DS saves. If it's `4`, it's a M01/NDS save.
    u32 version [[color("c371f0")]];
    std::assert(version == M01D_INDEX_VERSION, "Not a Korg M01D save file");

    u8 unknown01[8] [[color("404040")]];
    u32 numSongs [[color("ffe600")]];
    u8 unknown02[33] [[color("404040")]];
    u32 numSongsCopy [[color("ffe600")]];
    std::assert_warn(numSongs == numSongsCopy, "numSongs mismatched with numSongsCopy");
    u8 unknown03[16] [[color("404040")]];

    SongIdentifier songs[numSongs];

};

Header header @ 0x00;