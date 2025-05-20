//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/17/2025
//

#include "InstrumentHelper.h"

#include <iostream>
#include <ranges>
#include <utility>

const InstrumentMap defaultMap = {
    {
        "M1", {
            {
                "Keyboard", {
                    "Piano", "E.Piano1", "E.Piano2", "E.Piano3", "Clav", "Harpsicord", "Organ1", "Organ2", "MagicOrgan",
                    "DW-Piano", "DW-EP1", "DW-EP2", "DW-EP3", "DW-Clav", "DW-Organ1", "DW-Organ2"
                }
            },

            {
                "Guitar/Mallet", {
                    "Guitar1", "Guitar2", "E.Guitar", "Sitar1", "Sitar2", "Vibes", "Bell", "Tubular", "BellRing",
                    "Karimba", "SynMallet", "DW-Vibe", "DW-Bell"
                }
            },

            {
                "Bass", {
                    "A.Bass", "PickBass", "E.Bass", "Fretless", "SynBass1", "SynBass2", "SynBass3", "DW-Bass1",
                    "DW-Bass2"
                }
            },

            {
                "Strings/Pad", {
                    "Strings", "Voices", "Choir"
                }
            },

            {
                "Brass/Reed", {
                    "Flute", "PanFlute", "Bottles", "TenorSax", "Trumpet", "MuteTp", "TubaFlugel", "DoubleReed",
                    "Brass1", "Brass2"
                }
            },

            {
                "Lead Synth", {
                    "Wire", "SawWave", "SquareWave", "25%Pulse", "10%Pulse", "DW-Tri", "DW-Sine", "VoiceWave",
                    "DW-Voice"
                }
            },

            {
                "Poly Synth", {
                    "Universe", "SoftHorn", "SynBrass", "FvWave", "MvWave", "PanWave", "PingWave", "Digital1",
                    "Digital2", "Digital3", "Digital4", "Digital5", "Digital6"
                }
            },

            {
                "SE/Other", {
                    "KotoTrem", "BambooTrem", "Rhythm", "Lore", "FlexaTone", "WindBells"
                }
            },

            {
                "Hit/Chord", {
                    "Pole", "Pop", "MetalHit"
                }
            },

            {
                "Drum Kit", {
                    "DrumKit1", "DrumKit2", "DrumKit3", "PercKit", "NoiseKit"
                }
            },
        }
    },
    {
        "01/W", {
            {
                "Keyboard", {
                    "A.Piano", "SoftEP", "E.Piano", "PianoPad1", "PianoPad2", "SynPiano", "Clav", "Harpsicord",
                    "PercOrgan", "Organ1", "Organ2", "Rotary", "GospelOrg", "PipeOrgan1", "PipeOrgan2", "Accordion"
                }
            },
            {
                "Guitar/Mallet", {
                    "G.Guitar", "F.Guitar", "HardPick", "E.Guitar", "MuteGuitar", "DistGuitar", "FeedBacker", "Banjo",
                    "Harp", "Marimba", "Vibe", "MusicBox", "Gamelan", "DigiBell", "MetalBell", "VS Bell"
                }
            },
            {
                "Bass", {
                    "A.Bass1", "A.Bass2", "Fretless", "E.Bass1", "E.Bass2", "E.Bass3", "SlapBass", "SynthBass1",
                    "SynthBass2",
                    "TechBass", "BowBowBass", "RezzzzBass", "Residrops"
                }
            },
            {
                "Strings/Pad", {
                    "Violin", "Cello", "Pizzicato", "StringEns", "AnaStrings", "Choir", "SoftChoir", "Ahhs", "AirVox",
                    "SynVox"
                }
            },
            {
                "Brass/Reed", {
                    "Trumpet", "Trombone", "MuteTp", "HardFlute", "TinFlute", "BassonOboe", "Clarinet", "SopranoSax",
                    "AltoSax", "BariSax", "Tuba/FrH", "Harmonica", "BrassEns1", "BrassEns2"
                }
            },
            {
                "Lead Synth", {
                    "MonoLead", "MiniLead", "VS 89", "4%Pulse", "SynSine", "Sine"
                }
            },
            {
                "Poly Synth", {
                    "RawDeal", "Detune", "EtherBell", "FreshAir", "Ghostly", "Ephemerals", "AliaBass", "UnderWater",
                    "Spectrum1", "Spectrum2", "Spectrum3"
                }
            },
            {
                "SE/Other", {
                    "Stadium", "Thing", "TriRoll", "Clicker", "Crickets1", "Crickets2", "MagicBell", "IronUp", "Tooter",
                    "FluteFX", "Flutter"
                }
            },
            {
                "Hit/Chord", {
                    "OrchHit", "VibeHit", "Gong", "Timpani", "OrchPerc", "SynClaves", "SynTom1", "SynTom2", "Zap1",
                    "Zap2", "Industry1", "Industry2", "RevThing"
                }
            },
            {
                "Drum Kit", {
                    "TotalKit", "RockKit", "DanceKit", "AnalogKit", "HipHopKit", "R&BKit", "BD&SDKit1", "BD&SDKit2",
                    "TomKit", "CymbalKit", "PercKit1", "PercKit2", "SEKit"
                }
            },
        }
    },
    {
        "EX", {

            {
                "Keyboard", {
                    "ElecGrand", "E.Piano1", "E.Piano2", "ToyPiano", "Organ", "VoxOrgan", "Musette"
                }
            },

            {
                "Guitar/Mallet", {
                    "ChorusGtr", "JazzGuitar", "DistGuitar", "SitarTambr", "Shamisen", "Koto", "Glocken", "SteelDrum",
                    "BottlePop"
                }
            },

            {
                "Bass", {
                    "E.Bass", "SlapBass", "BoostSaw", "ElectroBs", "DarkBass", "FilterBass", "FatBass", "SawRezBass",
                    "SquRezBass", "DiscoBass", "VPMBass1", "VPMBass2", "AttackBass", "AcidDistBs", "DetuneBass",
                    "WobbleBass"
                }
            },

            {
                "Strings/Pad", {
                    "Strings", "StrQuartet", "TapeString", "AahChoir", "OohChoir", "VocoderPad", "AnalogStr1",
                    "AnalogStr2", "DarkPad", "NoisePad", "AnalogPad", "SquarePad", "5thPad", "OctavePad"
                }
            },

            {
                "Brass/Reed", {
                    "Trumpet", "TinWhistle", "TapeFlute", "Shakuhachi", "AltoSax", "TenorSax", "Bagpipe", "BrassEns"
                }
            },

            {
                "Lead Synth", {
                    "SoftLead", "UrbanLead", "HiResoLead", "MS20Lead", "OctaveLead", "DriveLead", "RaveLead",
                    "DualSquare", "SynWire1", "SynWire2", "SyncLoop", "5thSine", "5thSaw", "5thSquare", "ShortArp"
                }
            },

            {
                "Poly Synth", {
                    "DetuneStab", "UnisonStab", "5thStab", "PolyComp", "SquDetune", "VPMBrass", "SynthHorn",
                    "DarkSynth", "NoisyComp", "RisingPad", "TremoloSin", "TrillPad", "LPFSweep", "HPFSweep",
                    "WaveSweep", "Palawan"
                }
            },

            {
                "SE/Other", {
                    "Applause", "HeartBeat", "GunShot", "CarSFX", "Stream", "Forest", "ShakerLoop", "Noise",
                    "8bitNoise", "NoiseShot", "NoiseSplit", "SweepSplit", "SirenUp", "SirenDown", "Modulation",
                    "Signal"
                }
            },

            {
                "Hit/Chord", {
                    "M1PfChord", "EPChord", "min7Organ", "GtrChord1", "GtrChord2", "PowerChord", "StrChord", "Maj7Pad",
                    "SynChord1", "SynChord2", "SynChord3", "SynChord4", "Glissando", "OrchHits", "DanceHits"
                }
            },

            {
                "Drum Kit", {
                    "DDD1Kit", "DDM110Kit", "S3Kit", "LynKit", "StandrdKit", "HouseKit", "ElectroKit", "MinimalKit",
                    "TronicaKit", "D&BKit", "R&BKit", "HipHopKit", "EthnicKit", "SEKit", "CartoonKit", "GtrElement"
                }
            },
        }
    }

};

constexpr int kNumberOfDrumNotes = 12;
constexpr int kDrumLowestNote = 60;
constexpr int kNoteOffset = 0x80;

InstrumentHelper::InstrumentHelper() : instrumentMap(defaultMap)
{
}

InstrumentName InstrumentHelper::getInstrumentName(const uint8_t bankId, const uint8_t subBankId,
                                                   const uint8_t programId) const
{
    if (bankId > kNumberOfBanks || subBankId > kNumberOfSubBanks)
    {
        return InstrumentName("Error", "Error", "Error");
    }
    auto& bankName = kBankNames[bankId];
    auto& subBankName = kSubBankNames[subBankId];
    if (!instrumentMap.contains(bankName))
    {
        return InstrumentName("Unknown", "Unknown", "Unknown");
    }

    const auto& bank = instrumentMap.at(bankName);
    if (!bank.contains(subBankName))
    {
        return InstrumentName(bankName, "Unknown", "Unknown");
    }

    const auto& subBank = bank.at(subBankName);
    if (programId > subBank.size())
    {
        return InstrumentName(bankName, subBankName, "Unknown");
    }

    return InstrumentName(bankName, subBankName, subBank[programId]);
}

InstrumentIds InstrumentHelper::getProgramChangeIds(const std::string& instrumentName) const
{
    for (const auto& [bankName, subBanks] : instrumentMap)
    {
        for (const auto& [subBankName, instruments] : subBanks)
        {
            for (int programId = 0; programId < instruments.size(); ++programId)
            {
                if (instruments[programId] == instrumentName)
                {
                    uint8_t bankId = 0;
                    uint8_t subBankId = 0;
                    for (const auto& name : std::views::keys(instrumentMap))
                    {
                        if (name == bankName)
                        {
                            break;
                        }
                        ++bankId;
                    }
                    for (const auto& name : std::views::keys(subBanks))
                    {
                        if (name == subBankName)
                        {
                            break;
                        }
                        ++subBankId;
                    }
                    return InstrumentIds(bankId, subBankId, programId);
                }
            }
        }
    }
    return {};
}

YAML::Node InstrumentHelper::getInstrumentConfig(const YAML::Node& config, const uint8_t& bankId,
                                                 const uint8_t& subBankId, const uint8_t& programId) const
{
    const auto& [bank, subBank, program] = getInstrumentName(bankId, subBankId, programId);
    return getInstrumentConfig(config, bank, subBank, program);
}

YAML::Node InstrumentHelper::getInstrumentConfig(const YAML::Node& config, const std::string& bankName,
                                                 const std::string& subBankName, const std::string& programName)
{
    if (!config["Instruments"][bankName] || !config["Instruments"][bankName].IsMap()) return {};
    const auto& bankConfig = config["Instruments"][bankName];

    if (!bankConfig[subBankName] || !bankConfig[subBankName].IsMap()) return {};
    const auto& subBankConfig = bankConfig[subBankName];

    if (!subBankConfig[programName] || !subBankConfig[programName].IsMap()) return {};
    const auto& programConfig = subBankConfig[programName];

    return programConfig;
}

uint8_t InstrumentHelper::remapNoteNumber(const YAML::Node& instrumentConfig, const uint8_t& noteNumber)
{
    uint8_t realNoteNumber = noteNumber - kNoteOffset;

    if (instrumentConfig["transposition"])
    {
        realNoteNumber = std::max(0, std::min(127, realNoteNumber + instrumentConfig["transposition"].as<int>()));
    }

    if (!instrumentConfig["map"] || !(instrumentConfig["map"].IsSequence() || instrumentConfig["map"].IsMap()))
        return realNoteNumber;

    if (instrumentConfig["map"].IsSequence())
    {
        if (realNoteNumber < kDrumLowestNote || realNoteNumber >= kDrumLowestNote + kNumberOfDrumNotes)
        {
            return realNoteNumber;
        }

        const int index = (kNumberOfDrumNotes - 1) - (realNoteNumber - kDrumLowestNote);

        if (index < 0 || index >= instrumentConfig["map"].size()) return realNoteNumber;

        return instrumentConfig["map"][index].as<uint8_t>();
    }

    if (instrumentConfig["map"].IsMap())
    {
        if (!instrumentConfig["map"][realNoteNumber])
        {
            return realNoteNumber;
        }
        return instrumentConfig["map"][realNoteNumber].as<uint8_t>();
    }

    return realNoteNumber;
}
