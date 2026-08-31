#include <std/mem.pat>
#include <std/sys.pat>
#include <std/io.pat>

#pragma loop_limit 1000000

bool VALIDATE_CHECKSUMS = true;

enum M1Keyboard : u8 {
    Piano, EPiano1, EPiano2, EPiano3, Clav, Harpsicord, Organ1, Organ2, MagicOrgan, DWPiano, DWEP1, DWEP2, DWEP3, DWClav, DWOrgan1, DWOrgan2
};

enum M1GuitarMallet : u8 {
    Guitar1, Guitar2, EGuitar, Sitar1, Sitar2, Vibes, Bell, Tubular, BellRing, Karimba, SynMallet, DWVibe, DWBell
};

enum M1Bass : u8 {
    ABass, PickBass, EBass, Fretless, SynBass1, SynBass2, SynBass3, DWBass1, DWBass2
};

enum M1StringsPad : u8 {
    Strings, Voices, Choir
};

enum M1BrassReed : u8 {
    Flute, PanFlute, Bottles, TenorSax, Trumpet, MuteTp, TubaFlugel, DoubleReed, Brass1, Brass2
};

enum M1LeadSynth : u8 {
    Wire, SawWave, SquareWave,
    QuarterPulse, // 25%Pulse
    TenthPulse, // 10%Pulse
    DWTri, DWSine, VoiceWave, DWVoice
};

enum M1PolySynth : u8 {
    Universe, SoftHorn, SynBrass, FvWave, MvWave, PanWave, PingWave, Digital1, Digital2, Digital4, Digital5, Digital6
};

enum M1SeOther : u8 {
    KotoTrem, BambooTrem, Rhythm, Lore, FlexaTone, WindBells
};

enum M1HitChord : u8 {
    Pole, Pop, MetalHit
};

enum M1DrumKit : u8 {
    DrumKit1, DrumKit2, DrumKit3, PercKit, NoiseKit
};



enum K01WKeyboard : u8 {
    APiano, SoftEP, EPiano, PianoPad1, PianoPad2, SynPiano, Clav, Harpsicord, PercOrgan, Organ1, Organ2, Rotary, GospelOrg, PipeOrgan1, PipeOrgan2, Accordion
};

enum K01WGuitarMallet : u8 {
    GGuitar, FGuitar, HardPick, EGuitar, MuteGuitar, DistGuitar, FeedBacker, Banjo, Harp, Marimba, Vibe, MusicBox, Gamelan, DigiBell, MetalBell, VSBell
};

enum K01WBass : u8 {
    ABass1, ABass2, Fretless, EBass1, EBass2, SlapBass, SynthBass1, SynthBass2, TechBass, BowBowBass, RezzzzBass, Residrops
};

enum K01WStringsPad : u8 {
    Violin, Cello, Pizzicato, StringEns, AnaStrings, Choir, SoftChoir, Ahhs, AirVox, SynVox
};

enum K01WBrassReed : u8 {
    Trumpet, Trombone, MuteTp, HardFlute, TinFlute, BassonOboe, Clarinet, SopranoSax, AltoSax, BariSax, TubaFrH, Harmonica, BrassEns1, BrassEns2
};

enum K01WLeadSynth : u8 {
    MonoLead, MiniLead, VS89,
    FourPulse, // 4%Pulse
    SynSine, Sine
};

enum K01WPolySynth : u8 {
    RawDeal, Detune, EtherBell, FreshAir, Ghostly, Ephemerals, AliaBass, UnderWater, Spectrum1, Spectrum2, Spectrum3
};

enum K01WSeOther : u8 {
    Stadium, Thing, TriRoll, Clicker, Crickets1, Crickets2, MagicBell, IronUp, Tooter, FluteFX, Flutter
};

enum K01WHitChord : u8 {
    OrchHit, VibeHit, Gong, Timpani, OrchPerc, SynClaves, SynTom1, SynTom2, Zap1, Zap2, Industry1, Industry2, RevThing
};

enum K01WDrumKit : u8 {
    TotalKit, RockKit, DanceKit, AnalogKit, HipHopKit, RnBKit, BDnSDKit1, BDnSDKit2, TomKit, CymbalKit, PercKit1, PercKit2, SEKit
};



enum EXKeyboard : u8 {
    ElecGrand, EPiano1, EPiano2, ToyPiano, Organ, VoxOrgan, Musette
};

enum EXGuitarMallet : u8 {
    ChorusGtr, JazzGuitar, DistGuitar, SitarTambr, Shamisen, Koto, Glocken, SteelDrum, BottlePop
};

enum EXBass : u8 {
    EBass, SlapBass, BoostSaw, ElectroBs, DarkBass, FilterBass, FatBass, SawRezBass, SquRezBass, DiscoBass, VPMBass1, VPMBass2, AttackBass, AcidDistBs, DetuneBass, WobbleBass
};

enum EXStringsPad : u8 {
    Strings, StrQuartet, TapeString, AahChoir, OohChoir, VocoderPad, AnalogStr1, AnalogStr2, DarkPad, NoisePad, AnalogPad, SquarePad, FifthPad, OctavePad
};

enum EXBrassReed : u8 {
    Trumpet, TinWhistle, TapeFlute, Shakuhachi, AltoSax, TenorSax, Bagpipe, BrassEns
};

enum EXLeadSynth : u8 {
    SoftLead, UrbanLead, HiResoLead, MS20Lead, OctaveLead, DriveLead, RaveLead, DualSquare, SynWire1, SynWire2, SyncLoop, FifthSine, FifthSaw, FifthSquare, ShortArp
};

enum EXPolySynth : u8 {
    DetuneStab, UnisonStab, FifthStab, PolyComp, SquDetune, VPMBrass, SynthHorn, DarkSynth, NoisyComp, RisingPad, TremoloSin, TrillPad, LPFSweep, HPFSweep, WaveSweep, Palawan
};

enum EXSeOther : u8 {
    Applause, HeartBeat, GunShot, CarSFX, Stream, Forest, ShakerLoop, Noise, EightBitNoise, NoiseShot, NoiseSplit, SweepSplit, SirenUp, SirenDown, Modulation, Signal
};

enum EXHitChord : u8 {
    M1PfChord, EPChord, min7Organ, GtrChord1, GtrChord2, PowerChord, StrChord, Maj7Pad, SynChord1, SynChord2, SynChord3, SynChord4, Glissando, OrchHits, DanceHits
};

enum EXDrumKit : u8 {
    DDD1Kit, DDM110Kit, S3Kit, LynKit, StandrdKit, HouseKit, ElectroKit, MinimalKit, TronicaKit, DnBKit, RnBKit, HipHopKit, EthnicKit, SEKit, CartoonKit, GtrElement
};


enum Bank : u8 {
    M1, K01W, EX
};

enum Category : u8 {
    Keyboard, GuitarMallet, Bass, StringsPad, BrassReed, LeadSynth, PolySynth, SeOther, HitChord, DrumKit
};

enum KaosMode : u8 {
    MONO, CHD2, CHD3, CHD4, DRUM
};

enum Scale : u8 {
    CHROMA, IONIAN, DORIAN, PHRYGI, LYDIAN, MIXLYD, AEOLIA, LOCRIA, MBLUES, mBLUES, DIM, COMDIM, MPENTA, mPENTA, RAGA1, RAGA2, ARABIA, SPAIN, GYPSY, EGYPT, HAWAII, PELOG, JAPAN, RYUKYU, WHOLE, m3RD, M3RD, FOURTH, FIFTH, OCTAVE
};

enum DrumPattern : u8 {
    EIGHT_BEAT1, EIGHT_BEAT2, SIXTEEN_BEAT1, SIXTEEN_BEAT2, ROCK1, ROCK2, ROCK3, FUNK, HOUSE1, HOUSE2, ELECTRO, MINIMAL, DnB, RnB, HIPHOP, PERC
};

bitfield PlaybackBitfield {
    hasFX : 1 [[color("ff00ff")]];
    muted : 1 [[color("ff0000")]];
    soloed : 1 [[color("ffff00")]];
    kaosKey : 5 [[color("00ffff")]]; // 0-11 are C-B. Enums cannot be represented in bitfields
};

struct PlaybackState {
    PlaybackBitfield playbackBitfield [[color("ffff00")]];
    Scale kaosScale [[color("018500")]];
    DrumPattern kaosDrumPattern [[color("004E85")]];
    KaosMode kaosMode [[color("BD3600")]];
    u8 keyboardOctave [[color("65F7B3")]];
    u8 unknown01[0x01] [[color("404040")]];
};

bitfield DrumSampleMix {
    level   : 4 [[color("ff9178")]];
    panning : 4 [[color("ff00ff")]];
};

struct DrumInfo {
    DrumSampleMix mix;
    s8 timestretch [[color("f6ff78")]];
};

struct Instrument {
    Bank bank [[color("dd00ff")]];
    Category category [[color("ff8000")]];
    match (bank) {
        (Bank::M1): {
            match (category) {
                (Category::Keyboard): M1Keyboard program [[color("ffff00")]];
                (Category::GuitarMallet): M1GuitarMallet program [[color("ffff00")]];
                (Category::Bass): M1Bass program [[color("ffff00")]];
                (Category::StringsPad): M1StringsPad program [[color("ffff00")]];
                (Category::BrassReed): M1BrassReed program [[color("ffff00")]];
                (Category::LeadSynth): M1LeadSynth program [[color("ffff00")]];
                (Category::PolySynth): M1PolySynth program [[color("ffff00")]];
                (Category::SeOther): M1SeOther program [[color("ffff00")]];
                (Category::HitChord): M1HitChord program [[color("ffff00")]];
                (Category::DrumKit): M1DrumKit program [[color("ffff00")]];
                (_): u8 program [[color("ffff00")]];
            }
        }
        (Bank::K01W): {
            match (category) {
                (Category::Keyboard): K01WKeyboard program [[color("ffff00")]];
                (Category::GuitarMallet): K01WGuitarMallet program [[color("ffff00")]];
                (Category::Bass): K01WBass program [[color("ffff00")]];
                (Category::StringsPad): K01WStringsPad program [[color("ffff00")]];
                (Category::BrassReed): K01WBrassReed program [[color("ffff00")]];
                (Category::LeadSynth): K01WLeadSynth program [[color("ffff00")]];
                (Category::PolySynth): K01WPolySynth program [[color("ffff00")]];
                (Category::SeOther): K01WSeOther program [[color("ffff00")]];
                (Category::HitChord): K01WHitChord program [[color("ffff00")]];
                (Category::DrumKit): K01WDrumKit program [[color("ffff00")]];
                (_): u8 program [[color("ffff00")]];
            }
        }
        (Bank::EX): {
            match (category) {
                (Category::Keyboard): EXKeyboard program [[color("ffff00")]];
                (Category::GuitarMallet): EXGuitarMallet program [[color("ffff00")]];
                (Category::Bass): EXBass program [[color("ffff00")]];
                (Category::StringsPad): EXStringsPad program [[color("ffff00")]];
                (Category::BrassReed): EXBrassReed program [[color("ffff00")]];
                (Category::LeadSynth): EXLeadSynth program [[color("ffff00")]];
                (Category::PolySynth): EXPolySynth program [[color("ffff00")]];
                (Category::SeOther): EXSeOther program [[color("ffff00")]];
                (Category::HitChord): EXHitChord program [[color("ffff00")]];
                (Category::DrumKit): EXDrumKit program [[color("ffff00")]];
                (_): u8 program [[color("ffff00")]];
            }
        }
        (_): {
            u8 program [[color("ffff00")]];
        }
    }
    KaosMode kaosMode [[color("BD3600")]]; // Copy of the one in the PlaybackState, except MONO if it's a drum track.
    s8 kaosVariation [[color("850018")]];
    u8 attack [[color("2bff00")]]; // MIDI CC: 73
    u8 release [[color("00ffd9")]]; // MIDI CC: 72
    u8 volume [[color("004cff")]]; // MIDI CC: 7
    s8 panning [[color("ff00ff")]]; // MIDI CC: 10
    PlaybackState playbackState [[color("ffff00")]];
    std::assert(
        (playbackState.kaosMode == KaosMode::DRUM) ||
        (playbackState.kaosMode == kaosMode),
        std::format("{}::{}::{} : Kaos mode mismatch! header {}, body {}", bank, category, program, kaosMode, playbackState.kaosMode)
    );
    // 12 drum samples, each with parameters
    DrumInfo drumInfo[12];
    u8 unknown03[0x11] [[color("404040")]];
};



bitfield PlaybackInfo {
    hasSolo : 1;
    unknown01 : 7;
};

enum ReverbType : u8 {
    Hall,
    Room,
    Sprg
};

struct ReverbInfo {
    ReverbType type [[color("78ffe6")]];
    u8 time [[color("f6ff78")]];
    u8 level [[color("ff9178")]];
};

struct DelayInfo {
    bool syncOn [[color("78ffe6")]];
    u8 time [[color("f6ff78")]];
    s8 panRatio [[color("ff00ff")]];
    u8 feedback [[color("ff4400")]];
    u8 level [[color("ff9178")]];
};

bitfield SceneState {
    isReverbSelected : 1; // If false, then effect is Delay.
    unknown01 : 1;
    sceneLocked : 1;
    swing : 8;
    unknown02 : 5;
};

struct MasterInfo {
    u8 numTracks [[color("D837DB")]];
    u8 numMeasures [[color("DB37A4")]];
    u16 tempo [[color("00ff08")]];
    u8 swing [[color("c371f0")]];
    u8 step [[color("ff8400")]];
    u8 unknown02[0x02] [[color("404040")]];
};

struct MeasureInfo {
    u16 tempo [[color("00ff08")]];
    u8 step [[color("ff8400")]];
    u8 unknown01[0x05] [[color("404040")]];
};

fn offsetNote(u8 value) {
    return value - 0x80;
};

struct NoteData {
    u8 length [[color("6fed26")]];
    u8 velocity [[color("00ddff")]];
    u8 pitch [[transform("offsetNote"), color("f700ff")]];
    u8 startPoint [[color("ff0000")]];
};

struct PatternData {
    u8 measureNumber [[color("ff6600")]];
    u8 trackNumber [[color("ffa200")]];
    u16 numberOfNotes [[color("ffe600")]];
    NoteData notes[numberOfNotes];
};

fn sum_range(u128 offsetFrom, u128 offsetTo) {
    u32 acc = 0;
    for (u128 i = offsetFrom, i < offsetTo, i = i + 1) {
        acc = (acc + std::mem::read_unsigned(i, 1)) & 0xFFFFFFFF;
    }
    return acc;
};

struct SongChecksum<auto Start, auto Length> {
    u32 checksum [[color("B52D1B")]];
    u32 computed = 0;
    if (VALIDATE_CHECKSUMS) {
        computed = sum_range(Start + 4, Start + Length);
        std::assert_warn(checksum == computed, std::format("Song checksum mismatch at {:04X}:. Stored: {:08X}, Computed: {:08X}", Start, checksum, computed));
    }
};

enum Tag : u16 {
    End = 0x0000,
    MasterInfo = 0x0201,
    MeasureInfo = 0x0104,
    PatternData = 0x0105,
};

u8 g_numTracks = 0;
u8 g_numMeasures = 0;

struct Chunk {
    Tag tag [[color("00FFF4")]];
    u16 length [[color("c9c9c9")]];
    u64 start = $;
    match (tag) {
        (Tag::MasterInfo): {
            std::assert_warn(g_numTracks == 0 && g_numMeasures == 0, "found two MasterInfo chunks in one song");
            MasterInfo masterInfo;
            g_numTracks = masterInfo.numTracks;
            g_numMeasures = masterInfo.numMeasures;
        }
        (Tag::MeasureInfo): {
            MeasureInfo measureInfo[length / sizeof(MeasureInfo)];
            std::assert_warn(g_numMeasures != 0, "MeasureInfo chunk found before MasterInfo");
            std::assert_warn(g_numMeasures == length / sizeof(MeasureInfo), "mismatched measure number and length");
        }
        (Tag::PatternData): {
            PatternData patternData;
            std::assert_warn(patternData.trackNumber < g_numTracks, "track number out of range");
            std::assert_warn(patternData.measureNumber < g_numMeasures, "measure number out of range");
        }
        (_): u8 data[length] [[color("0000FF")]];
    }

    std::assert($ == start + length, std::format("{} chunk at {:04X}: decoded length {}, should be {}", tag, start - 4, $ - start, length));

    if (tag == Tag::End) {
        break;
    }
};

struct SongData<auto Start, auto Length> {
    g_numTracks = 0;
    g_numMeasures = 0;

    SongChecksum<Start, Length> checksum;
    // Similar to the version in the song header, this is always `4` for M01/NDS,
    // and always `5` for the M01D/3DS.
    u32 version [[color("c371f0")]];
    Instrument instruments[8];
    PlaybackInfo playbackInfo [[color("bffc56")]];
    u8 unknown01[3] [[color("404040")]];
    ReverbInfo reverbInfo;
    u8 unknown02[1] [[color("404040")]];
    DelayInfo delayInfo;
    u8 unknown03[3] [[color("404040")]];
    SceneState sceneState [[color("fba6ff")]];
    u8 unknown04[2] [[color("404040")]];
    // The song name for factory songs, like Demo 2 and both Bonus tracks.
    // Demo 1 doesn't have this field.
    char factorySongName[8] [[color("ff00ff")]];
    u8 unknown05[12] [[color("404040")]];
    char songLabel[4] [[color("0091ff")]];
    std::assert(songLabel == "song", "Song is missing data label");
    u64 songDataStart = $;
    u32 songDataLength [[color("c9c9c9")]]; // Includes the length value bytes
    u32 songDataVersion [[color("c371f0")]];
    u8 unknown06[4] [[color("404040")]];
    Chunk chunks[while($ < Start + Length)];
    std::assert(Start + Length == songDataStart + songDataLength, "mismatch in song length values");
    std::assert($ == Start + Length, "song data did not end at the right length");
};

struct SongIdentifier<auto FollowSongs> {
    bool songHasData [[color("00ff00")]];
    char name[8] [[color("ff00ff")]];
    u8 unknown01[0x0F] [[color("404040")]];
    u32 songStartAddress [[color("00ffff")]];
    u32 songLength [[color("0091ff")]];
    u8 unknown02[0x08] [[color("404040")]];
    if (songHasData && FollowSongs) {
        SongData<songStartAddress, songLength> song @ songStartAddress;
    }
};

fn check_header_sum(u32 stored) {
    u32 computed = 0;
    bool valid = false;
    if (VALIDATE_CHECKSUMS) {
        // Sum of all bytes from immediately after the checksum u32 until
        // 0x1C4 (exclusive), which is where the block of \xFF starts.
        computed = sum_range($ + 0x04, $ + 0x1C4);
        valid = stored == computed;
        std::assert_warn(valid, "header checksum mismatch");
    }
    return std::format("{:08X} {}", stored, valid ? "(OK)" : (VALIDATE_CHECKSUMS ? std::format("(BAD, want {:08X})", computed) : ""));
};
using HeaderChecksum = u32 [[format("check_header_sum")]];

struct Header<auto ValidateChecksum, auto FollowSongs> {
    if (ValidateChecksum) {
        HeaderChecksum checksum [[color("B52D1B")]];
    } else {
        u32 checksum [[color("871E10")]];
    }
    char signature[4] [[color("0091ff")]];
    std::assert(signature == "M01W", "Not a Korg M01 save file");
    // always `4` for M01/NDS saves, and always `7` for M01D/3DS saves.
    u32 version [[color("c371f0")]];

    SongIdentifier<FollowSongs> songs[10];

    // There's the exact amount of space needed for an "11th" song, but
    // none in game. I'm assuming it's an unused slot.
    SongIdentifier<false> unusedSlot;
};

Header<true, true> header @ 0x00;

// This is an almost direct copy of the header. Likely as a backup if the first one gets messed up.
//
// The only difference is that sometimes the checksum field reads ascii "BFBA", the same string that
// shows up right before the start of the song data range.
Header<false, false> copyHeader @ 0x0204;