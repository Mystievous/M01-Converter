#pragma description Korg M01D (3DS) song data file
#pragma magic [ 73 6F 6E 67 ] @ 0x1FC

#include <M01_Song.pat>

bool VALIDATE_CHECKSUMS = true;

u32 g_songLength = std::mem::read_unsigned(0x200, 4) + 0x200;
SongData<0x00, g_songLength, VALIDATE_CHECKSUMS> song @ 0x00;
