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

enum SubBank : u8 {
    Keyboard, GuitarMallet, Bass, StringsPad, BrassReed, LeadSynth, PolySynth, SeOther, HitChord, DrumKit
};


bitfield PlaybackState {
    hasFX : 1 [[color("00ddff")]];
    muted : 1 [[color("ff0000")]];
    soloed : 1 [[color("ffff00")]];
};

struct DrumInfo {
    u8 level [[color("ff9178")]];
    s8 timestretch [[color("f6ff78")]];
    s8 panning [[color("ff00ff")]];
};


struct Instrument {
    padding[0x08];
    Bank bank [[color("dd00ff")]];
    SubBank subBank [[color("ff8000")]];
    match (bank) {
        (Bank::M1): {
            match (subBank) {
                (SubBank::Keyboard): M1Keyboard program [[color("ffff00")]];
                (SubBank::GuitarMallet): M1GuitarMallet program [[color("ffff00")]];
                (SubBank::Bass): M1Bass program [[color("ffff00")]];
                (SubBank::StringsPad): M1StringsPad program [[color("ffff00")]];
                (SubBank::BrassReed): M1BrassReed program [[color("ffff00")]];
                (SubBank::LeadSynth): M1LeadSynth program [[color("ffff00")]];
                (SubBank::PolySynth): M1PolySynth program [[color("ffff00")]];
                (SubBank::SeOther): M1SeOther program [[color("ffff00")]];
                (SubBank::HitChord): M1HitChord program [[color("ffff00")]];
                (SubBank::DrumKit): M1DrumKit program [[color("ffff00")]];
                (_): u8 program [[color("ffff00")]];
            }
        }
        (Bank::K01W): {
            match (subBank) {
                (SubBank::Keyboard): K01WKeyboard program [[color("ffff00")]];
                (SubBank::GuitarMallet): K01WGuitarMallet program [[color("ffff00")]];
                (SubBank::Bass): K01WBass program [[color("ffff00")]];
                (SubBank::StringsPad): K01WStringsPad program [[color("ffff00")]];
                (SubBank::BrassReed): K01WBrassReed program [[color("ffff00")]];
                (SubBank::LeadSynth): K01WLeadSynth program [[color("ffff00")]];
                (SubBank::PolySynth): K01WPolySynth program [[color("ffff00")]];
                (SubBank::SeOther): K01WSeOther program [[color("ffff00")]];
                (SubBank::HitChord): K01WHitChord program [[color("ffff00")]];
                (SubBank::DrumKit): K01WDrumKit program [[color("ffff00")]];
                (_): u8 program [[color("ffff00")]];
            }
        }
        (Bank::EX): {
            match (subBank) {
                (SubBank::Keyboard): EXKeyboard program [[color("ffff00")]];
                (SubBank::GuitarMallet): EXGuitarMallet program [[color("ffff00")]];
                (SubBank::Bass): EXBass program [[color("ffff00")]];
                (SubBank::StringsPad): EXStringsPad program [[color("ffff00")]];
                (SubBank::BrassReed): EXBrassReed program [[color("ffff00")]];
                (SubBank::LeadSynth): EXLeadSynth program [[color("ffff00")]];
                (SubBank::PolySynth): EXPolySynth program [[color("ffff00")]];
                (SubBank::SeOther): EXSeOther program [[color("ffff00")]];
                (SubBank::HitChord): EXHitChord program [[color("ffff00")]];
                (SubBank::DrumKit): EXDrumKit program [[color("ffff00")]];
                (_): u8 program [[color("ffff00")]];
            }
        }
        (_): {
            u8 program [[color("ffff00")]];
        }
    }
    padding[0x02];
    u8 attack [[color("2bff00")]];
    u8 release [[color("00ffd9")]];
    u8 volume [[color("004cff")]];
    s8 panning [[color("ff00ff")]];
    PlaybackState playbackState [[color("ffff00")]]; 
    padding[0x05];
    DrumInfo drumInfo[8];
    padding[0x09];
};



bitfield PlaybackInfo {
    hasSolo : 1;
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
    padding[3];
};

bitfield SceneState {
    reverbOn : 1;
    padding : 1;
    sceneLocked : 1;
};

struct MasterInfo {
    u16 tempo [[color("00ff08")]];
    u8 swing [[color("c371f0")]];
    u8 step [[color("ff8400")]];
};

struct MeasureInfo {
    u16 tempo [[color("00ff08")]];
    u8 step [[color("ff8400")]];
    padding[5];
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

struct BlockInfo {
    padding[1];
    padding[1];
    u8 blockLength [[color("c9c9c9")]];
    padding[1];
};

struct TrackData {
    BlockInfo blockInfo;
    u8 measureNumber [[color("ff6600")]];
    u8 trackNumber [[color("ffa200")]];
    u8 numberOfNotes [[color("ffe600")]];
    padding[1];
    NoteData notes[numberOfNotes];
};

struct MeasureData {
    TrackData trackData[8];
};

struct SongData {
    Instrument instruments[8];
    padding[8];
    PlaybackInfo playbackInfo [[color("bffc56")]];
    padding[1];
    padding[2];
    ReverbInfo reverbInfo;
    padding[1];
    DelayInfo delayInfo;
    SceneState sceneState [[color("fba6ff")]];
    padding[45];
    MasterInfo masterInfo;
    padding[6];
    MeasureInfo measureInfo[99];
    MeasureData measureData[99];
};

struct SongIdentifier {
    bool songHasData [[color("00ff00")]];
    char name[8] [[color("ff00ff")]];
    padding[15];
    u32 songStartAddress [[color("00ffff")]];
    u16 songLength [[color("0091ff")]];
    padding[10];
    if (songHasData) {
        SongData song @ songStartAddress;
    }
};

SongIdentifier songs[10] @ 0x0C;