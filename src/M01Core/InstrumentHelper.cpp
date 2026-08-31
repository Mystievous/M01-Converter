//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/17/2025
//

#include "M01Core/InstrumentHelper.h"

#include <format>
#include <iostream>
#include <ranges>
#include <utility>
#include <stdexcept>

#include <yaml-cpp/yaml.h>

const InstrumentMap defaultMap = {
    {
        "M1", {
            {
                "Keyboard",
                {
                    "Piano", "E.Piano1", "E.Piano2", "E.Piano3", "Clav", "Harpsicord", "Organ1", "Organ2", "MagicOrgan",
                    "DW-Piano", "DW-EP1", "DW-EP2", "DW-EP3", "DW-Clav", "DW-Organ1", "DW-Organ2"
                }
            },

            {
                "Guitar/Mallet",
                {
                    "Guitar1", "Guitar2", "E.Guitar", "Sitar1", "Sitar2", "Vibes", "Bell", "Tubular", "BellRing",
                    "Karimba", "SynMallet", "DW-Vibe", "DW-Bell"
                }
            },

            {
                "Bass",
                {"A.Bass", "PickBass", "E.Bass", "Fretless", "SynBass1", "SynBass2", "SynBass3", "DW-Bass1", "DW-Bass2"}
            },

            {"Strings/Pad", {"Strings", "Voices", "Choir"}},

            {
                "Brass/Reed",
                {
                    "Flute", "PanFlute", "Bottles", "TenorSax", "Trumpet", "MuteTp", "TubaFlugel", "DoubleReed",
                    "Brass1", "Brass2"
                }
            },

            {
                "Lead Synth",
                {"Wire", "SawWave", "SquareWave", "25%Pulse", "10%Pulse", "DW-Tri", "DW-Sine", "VoiceWave", "DW-Voice"}
            },

            {
                "Poly Synth",
                {
                    "Universe", "SoftHorn", "SynBrass", "FvWave", "MvWave", "PanWave", "PingWave", "Digital1",
                    "Digital2", "Digital3", "Digital4", "Digital5", "Digital6"
                }
            },

            {"SE/Other", {"KotoTrem", "BambooTrem", "Rhythm", "Lore", "FlexaTone", "WindBells"}},

            {"Hit/Chord", {"Pole", "Pop", "MetalHit"}},

            {"Drum Kit", {"DrumKit1", "DrumKit2", "DrumKit3", "PercKit", "NoiseKit"}},
        }
    },
    {
        "01/W", {
            {
                "Keyboard",
                {
                    "A.Piano", "SoftEP", "E.Piano", "PianoPad1", "PianoPad2", "SynPiano", "Clav", "Harpsicord",
                    "PercOrgan", "Organ1", "Organ2", "Rotary", "GospelOrg", "PipeOrgan1", "PipeOrgan2", "Accordion"
                }
            },
            {
                "Guitar/Mallet",
                {
                    "G.Guitar", "F.Guitar", "HardPick", "E.Guitar", "MuteGuitar", "DistGuitar", "FeedBacker", "Banjo",
                    "Harp", "Marimba", "Vibe", "MusicBox", "Gamelan", "DigiBell", "MetalBell", "VS Bell"
                }
            },
            {
                "Bass",
                {
                    "A.Bass1", "A.Bass2", "Fretless", "E.Bass1", "E.Bass2", "E.Bass3", "SlapBass", "SynthBass1",
                    "SynthBass2", "TechBass", "BowBowBass", "RezzzzBass", "Residrops"
                }
            },
            {
                "Strings/Pad",
                {
                    "Violin", "Cello", "Pizzicato", "StringEns", "AnaStrings", "Choir", "SoftChoir", "Ahhs", "AirVox",
                    "SynVox"
                }
            },
            {
                "Brass/Reed",
                {
                    "Trumpet", "Trombone", "MuteTp", "HardFlute", "TinFlute", "BassonOboe", "Clarinet", "SopranoSax",
                    "AltoSax", "BariSax", "Tuba/FrH", "Harmonica", "BrassEns1", "BrassEns2"
                }
            },
            {"Lead Synth", {"MonoLead", "MiniLead", "VS 89", "4%Pulse", "SynSine", "Sine"}},
            {
                "Poly Synth",
                {
                    "RawDeal", "Detune", "EtherBell", "FreshAir", "Ghostly", "Ephemerals", "AliaBass", "UnderWater",
                    "Spectrum1", "Spectrum2", "Spectrum3"
                }
            },
            {
                "SE/Other",
                {
                    "Stadium", "Thing", "TriRoll", "Clicker", "Crickets1", "Crickets2", "MagicBell", "IronUp", "Tooter",
                    "FluteFX", "Flutter"
                }
            },
            {
                "Hit/Chord",
                {
                    "OrchHit", "VibeHit", "Gong", "Timpani", "OrchPerc", "SynClaves", "SynTom1", "SynTom2", "Zap1",
                    "Zap2", "Industry1", "Industry2", "RevThing"
                }
            },
            {
                "Drum Kit",
                {
                    "TotalKit", "RockKit", "DanceKit", "AnalogKit", "HipHopKit", "R&BKit", "BD&SDKit1", "BD&SDKit2",
                    "TomKit", "CymbalKit", "PercKit1", "PercKit2", "SEKit"
                }
            },
        }
    },
    {
        "EX", {

            {"Keyboard", {"ElecGrand", "E.Piano1", "E.Piano2", "ToyPiano", "Organ", "VoxOrgan", "Musette"}},

            {
                "Guitar/Mallet",
                {
                    "ChorusGtr", "JazzGuitar", "DistGuitar", "SitarTambr", "Shamisen", "Koto", "Glocken", "SteelDrum",
                    "BottlePop"
                }
            },

            {
                "Bass",
                {
                    "E.Bass", "SlapBass", "BoostSaw", "ElectroBs", "DarkBass", "FilterBass", "FatBass", "SawRezBass",
                    "SquRezBass", "DiscoBass", "VPMBass1", "VPMBass2", "AttackBass", "AcidDistBs", "DetuneBass",
                    "WobbleBass"
                }
            },

            {
                "Strings/Pad",
                {
                    "Strings", "StrQuartet", "TapeString", "AahChoir", "OohChoir", "VocoderPad", "AnalogStr1",
                    "AnalogStr2", "DarkPad", "NoisePad", "AnalogPad", "SquarePad", "5thPad", "OctavePad"
                }
            },

            {
                "Brass/Reed",
                {"Trumpet", "TinWhistle", "TapeFlute", "Shakuhachi", "AltoSax", "TenorSax", "Bagpipe", "BrassEns"}
            },

            {
                "Lead Synth",
                {
                    "SoftLead", "UrbanLead", "HiResoLead", "MS20Lead", "OctaveLead", "DriveLead", "RaveLead",
                    "DualSquare", "SynWire1", "SynWire2", "SyncLoop", "5thSine", "5thSaw", "5thSquare", "ShortArp"
                }
            },

            {
                "Poly Synth",
                {
                    "DetuneStab", "UnisonStab", "5thStab", "PolyComp", "SquDetune", "VPMBrass", "SynthHorn",
                    "DarkSynth", "NoisyComp", "RisingPad", "TremoloSin", "TrillPad", "LPFSweep", "HPFSweep",
                    "WaveSweep", "Palawan"
                }
            },

            {
                "SE/Other",
                {
                    "Applause", "HeartBeat", "GunShot", "CarSFX", "Stream", "Forest", "ShakerLoop", "Noise",
                    "8bitNoise", "NoiseShot", "NoiseSplit", "SweepSplit", "SirenUp", "SirenDown", "Modulation", "Signal"
                }
            },

            {
                "Hit/Chord",
                {
                    "M1PfChord", "EPChord", "min7Organ", "GtrChord1", "GtrChord2", "PowerChord", "StrChord", "Maj7Pad",
                    "SynChord1", "SynChord2", "SynChord3", "SynChord4", "Glissando", "OrchHits", "DanceHits"
                }
            },

            {
                "Drum Kit",
                {
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

InstrumentName InstrumentHelper::GetInstrumentName(const InstrumentId id) const
{
    if (id.bank >= kNumberOfBanks || id.category >= kNumberOfSubBanks)
    {
        return InstrumentName("Error", "Error", "Error");
    }
    auto& bankName = kBankNames[id.bank];
    auto& subBankName = kSubBankNames[id.category];
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
    if (id.program >= subBank.size())
    {
        return InstrumentName(bankName, subBankName, "Unknown");
    }

    return InstrumentName(bankName, subBankName, subBank[id.program]);
}

// MIDI data bytes are 0-127, and channels 0-15.
static uint8_t ToRangedByte(const YAML::Node& node, const std::string& field, const int maxValue)
{
    const auto value = node.as<int>();
    if (value < 0 || value > maxValue)
    {
        throw std::runtime_error(std::format("{} must be 0-{}, got {}", field, maxValue, value));
    }
    return static_cast<uint8_t>(value);
}

static std::optional<uint8_t> ReadRangedByte(const YAML::Node& parent, const std::string& field, const int maxValue)
{
    const auto node = parent[field];
    if (!node)
    {
        return std::nullopt;
    }
    return ToRangedByte(node, field, maxValue);
}

void InstrumentHelper::LoadConfigFile(const std::string& configPath)
{
    config_.clear();
    configLoaded_ = false;

    YAML::Node root;
    try
    {
        root = YAML::LoadFile(configPath);
    }
    catch (const YAML::BadFile&)
    {
        throw std::runtime_error(std::format("Could not open config file '{}'.", configPath));
    }
    catch (const YAML::ParserException& e)
    {
        throw std::runtime_error(std::format("Config file '{}' is not valid YAML: {}", configPath, e.what()));
    }

    const auto instruments = root["Instruments"];
    if (!instruments)
    {
        throw std::runtime_error(std::format("Config file '{}' has no 'Instruments' section.", configPath));
    }

    int skippedEntries = 0;

    for (auto itBank = instruments.begin(); itBank != instruments.end(); ++itBank)
    {
        const auto bankName = itBank->first.as<std::string>();
        YAML::Node bankNode = itBank->second;

        for (auto itSub = bankNode.begin(); itSub != bankNode.end(); ++itSub)
        {
            const auto subBankName = itSub->first.as<std::string>();
            YAML::Node subNode = itSub->second;

            for (auto itProg = subNode.begin(); itProg != subNode.end(); ++itProg)
            {
                const auto programName = itProg->first.as<std::string>();
                YAML::Node progNode = itProg->second;

                try
                {
                    InstrumentConfig cfg;

                    cfg.bankMsb = ReadRangedByte(progNode, "bankMsb", 127);
                    cfg.bankLsb = ReadRangedByte(progNode, "bankLsb", 127);
                    cfg.program = ReadRangedByte(progNode, "program", 127);
                    cfg.channel = ReadRangedByte(progNode, "channel", 15);

                    if (progNode["transposition"])
                        cfg.transposition = progNode["transposition"].as<int>();

                    if (progNode["map"])
                    {
                        if (progNode["map"].IsSequence())
                        {
                            std::vector<uint8_t> seq;
                            for (const auto& v : progNode["map"])
                            {
                                seq.push_back(ToRangedByte(v, std::format("map entry {}", seq.size()), 127));
                            }
                            if (seq.size() != kNumberOfDrumNotes)
                            {
                                throw std::runtime_error(
                                    std::format("map must have {} entries, got {}", kNumberOfDrumNotes, seq.size()));
                            }
                            cfg.mapList = std::move(seq);
                        }
                        else if (progNode["map"].IsMap())
                        {
                            std::map<int, uint8_t> mp;
                            for (auto it = progNode["map"].begin(); it != progNode["map"].end(); ++it)
                            {
                                const auto key = ToRangedByte(it->first, "map key", 127);
                                mp.emplace(key, ToRangedByte(it->second, std::format("map value for {}", key), 127));
                            }
                            cfg.mapDict = std::move(mp);
                        }
                    }

                    config_[bankName][subBankName][programName] = std::move(cfg);
                }
                catch (const std::exception& e)
                {
                    std::cerr << std::format("Config: skipping {} / {} / {}: {}\n", bankName, subBankName, programName,
                                             e.what());
                    skippedEntries++;
                }
            }
        }
    }

    if (skippedEntries != 0)
    {
        std::cerr << std::format("Config file '{}': {} entries skipped.\n", configPath, skippedEntries);
    }

    configLoaded_ = true;
}

InstrumentConfig InstrumentHelper::GetInstrumentConfig(const std::string& bankName,
                                                       const std::string& categoryName,
                                                       const std::string& programName) const
{
    InstrumentConfig empty;
    if (!configLoaded_)
        return empty;

    const auto itBank = config_.find(bankName);
    if (itBank == config_.end())
        return empty;

    const auto itSub = itBank->second.find(categoryName);
    if (itSub == itBank->second.end())
        return empty;

    const auto itProg = itSub->second.find(programName);
    if (itProg == itSub->second.end())
        return empty;

    return itProg->second;
}

InstrumentConfig InstrumentHelper::GetInstrumentConfig(const InstrumentId id) const
{
    const auto& [bank, subBank, program] = GetInstrumentName(id);
    return GetInstrumentConfig(bank, subBank, program);
}

uint8_t InstrumentHelper::RemapNoteNumber(const InstrumentConfig& instrumentConfig, const int pitch)
{
    auto realNoteNumber = pitch;

    if (instrumentConfig.transposition.has_value())
    {
        realNoteNumber = std::max(0, std::min(127, realNoteNumber + instrumentConfig.transposition.value()));
    }

    if (!instrumentConfig.mapList.has_value() && !instrumentConfig.mapDict.has_value())
    {
        return realNoteNumber;
    }

    if (instrumentConfig.mapList.has_value())
    {
        const auto& list = *instrumentConfig.mapList;
        if (realNoteNumber < kDrumLowestNote || realNoteNumber >= kDrumLowestNote + kNumberOfDrumNotes)
        {
            return realNoteNumber;
        }

        const auto index = static_cast<size_t>((kNumberOfDrumNotes - 1) - (realNoteNumber - kDrumLowestNote));

        if (index >= list.size())
            return realNoteNumber;

        return list[index];
    }

    if (instrumentConfig.mapDict.has_value())
    {
        const auto& dict = *instrumentConfig.mapDict;
        if (!dict.contains(realNoteNumber))
        {
            return realNoteNumber;
        }
        return dict.at(realNoteNumber);
    }

    return realNoteNumber;
}

uint8_t InstrumentHelper::MapRange(const long& value, const long& min, const long& max, const long& newMin,
                                   const long& newMax)
{
    const long range = max - min;
    return static_cast<uint8_t>(((value - min) * (newMax - newMin) + range / 2) / range + newMin);
}
