#pragma description Korg M01 (NDS) save file
#pragma magic [ 4D 30 31 57 04 00 00 00 ] @ 0x04

#include <std/mem.pat>
#include <std/sys.pat>
#include <std/io.pat>

#include <M01_Song.pat>

bool VALIDATE_CHECKSUMS = true;
u32 M01_INDEX_VERSION = 0x04;

struct SongIdentifier<auto FollowSongs> {
    bool songHasData [[color("00ff00")]];
    char name[8] [[color("ff00ff")]];
    u8 padding01[0x0F] [[color("404040")]];
    u32 songStartAddress [[color("00ffff")]];
    u32 songLength [[color("0091ff")]];
    u8 padding02[0x08] [[color("404040")]];
    if (songHasData && FollowSongs) {
        SongData<songStartAddress, songLength, VALIDATE_CHECKSUMS> song @ songStartAddress;
    }
};

fn check_header_sum(u32 stored) {
    u32 computed = 0;
    bool valid = false;
    if (VALIDATE_CHECKSUMS) {
        // Sum of all bytes from immediately after the checksum u32 until
        // 0x1C4 (exclusive), which is where the block of \xFF starts.
        computed = sum_range(0x04, 0x1C4);
        valid = stored == computed;
        std::assert_warn(valid, "header checksum mismatch");
    }
    return std::format("{:08X} {}", stored, valid ? "(OK)" : (VALIDATE_CHECKSUMS ? std::format("(BAD, want {:08X})", computed) : ""));
};
using HeaderChecksum = u32 [[format("check_header_sum")]];

fn check_copy_header_sum(u32 stored) {
    if (stored == 0x41424642) { // "BFBA"
        return "BFBA (not written)";
    }
    u32 computed = 0;
    bool valid = false;
    if (VALIDATE_CHECKSUMS) {
        // Sum of all bytes from immediately after the checksum u32 until
        // 0x3C8 (exclusive), which is where the block of \xFF starts.
        computed = sum_range(0x208, 0x3C8);
        valid = stored == computed;
        std::assert_warn(valid, "header copy checksum mismatch");
    }
    return std::format("{:08X} {}", stored, valid ? "(OK)" : (VALIDATE_CHECKSUMS ? std::format("(BAD, want {:08X})", computed) : ""));
};
using HeaderCopyChecksum = u32 [[format("check_copy_header_sum")]];

struct Header<auto IsCopy, auto FollowSongs> {
    if (!IsCopy) {
        HeaderChecksum checksum [[color("B52D1B")]];
    } else {
        HeaderCopyChecksum checksum [[color("B52D1B")]];
    }
    char signature[4] [[color("0091ff")]];
    std::assert(signature == "M01W", "Not a Korg M01 save file");
    // always `4` for M01/NDS saves. If it's `7`, it's a M01D/3DS save instead.
    u32 version [[color("c371f0")]];
    std::assert(version == M01_INDEX_VERSION, "Not a Korg M01 save file");

    SongIdentifier<FollowSongs> songs[10];

    // There's the exact amount of space needed for an "11th" song, but
    // none in game. I'm assuming it's an unused slot.
    SongIdentifier<false> unusedSlot;
};

Header<false, true> header @ 0x00;

u8 padding01[0x40] @ 0x01C4 [[color("404040")]]; // filled with 0xFF

// This is an almost direct copy of the header. Likely as a backup if the first one gets messed up.
//
// The only difference is that sometimes the checksum field reads ascii "BFBA", the same string that
// shows up right before the start of the song data range.
Header<true, false> copyHeader @ 0x0204;

u8 padding02[0xC34] @ 0x3C8 [[color("404040")]]; // filled with 0xFF

char sectionBreak[4] @ 0x0FFC;
std::assert(sectionBreak == "BFBA", "section break not present before song data");
