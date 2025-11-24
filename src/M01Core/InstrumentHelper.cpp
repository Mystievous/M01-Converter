//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/17/2025
//

#include "M01Core/InstrumentHelper.h"

#include <iostream>
#include <ranges>
#include <utility>

#include <yaml-cpp/yaml.h>

const InstrumentMap defaultMap = {
    {"M1", {
               {"Keyboard", {"Piano", "E.Piano1", "E.Piano2", "E.Piano3", "Clav", "Harpsicord", "Organ1", "Organ2", "MagicOrgan", "DW-Piano", "DW-EP1", "DW-EP2", "DW-EP3", "DW-Clav", "DW-Organ1", "DW-Organ2"}},

               {"Guitar/Mallet", {"Guitar1", "Guitar2", "E.Guitar", "Sitar1", "Sitar2", "Vibes", "Bell", "Tubular", "BellRing", "Karimba", "SynMallet", "DW-Vibe", "DW-Bell"}},

               {"Bass", {"A.Bass", "PickBass", "E.Bass", "Fretless", "SynBass1", "SynBass2", "SynBass3", "DW-Bass1", "DW-Bass2"}},

               {"Strings/Pad", {"Strings", "Voices", "Choir"}},

               {"Brass/Reed", {"Flute", "PanFlute", "Bottles", "TenorSax", "Trumpet", "MuteTp", "TubaFlugel", "DoubleReed", "Brass1", "Brass2"}},

               {"Lead Synth", {"Wire", "SawWave", "SquareWave", "25%Pulse", "10%Pulse", "DW-Tri", "DW-Sine", "VoiceWave", "DW-Voice"}},

               {"Poly Synth", {"Universe", "SoftHorn", "SynBrass", "FvWave", "MvWave", "PanWave", "PingWave", "Digital1", "Digital2", "Digital3", "Digital4", "Digital5", "Digital6"}},

               {"SE/Other", {"KotoTrem", "BambooTrem", "Rhythm", "Lore", "FlexaTone", "WindBells"}},

               {"Hit/Chord", {"Pole", "Pop", "MetalHit"}},

               {"Drum Kit", {"DrumKit1", "DrumKit2", "DrumKit3", "PercKit", "NoiseKit"}},
           }},
    {"01/W", {
                 {"Keyboard", {"A.Piano", "SoftEP", "E.Piano", "PianoPad1", "PianoPad2", "SynPiano", "Clav", "Harpsicord", "PercOrgan", "Organ1", "Organ2", "Rotary", "GospelOrg", "PipeOrgan1", "PipeOrgan2", "Accordion"}},
                 {"Guitar/Mallet", {"G.Guitar", "F.Guitar", "HardPick", "E.Guitar", "MuteGuitar", "DistGuitar", "FeedBacker", "Banjo", "Harp", "Marimba", "Vibe", "MusicBox", "Gamelan", "DigiBell", "MetalBell", "VS Bell"}},
                 {"Bass", {"A.Bass1", "A.Bass2", "Fretless", "E.Bass1", "E.Bass2", "E.Bass3", "SlapBass", "SynthBass1", "SynthBass2", "TechBass", "BowBowBass", "RezzzzBass", "Residrops"}},
                 {"Strings/Pad", {"Violin", "Cello", "Pizzicato", "StringEns", "AnaStrings", "Choir", "SoftChoir", "Ahhs", "AirVox", "SynVox"}},
                 {"Brass/Reed", {"Trumpet", "Trombone", "MuteTp", "HardFlute", "TinFlute", "BassonOboe", "Clarinet", "SopranoSax", "AltoSax", "BariSax", "Tuba/FrH", "Harmonica", "BrassEns1", "BrassEns2"}},
                 {"Lead Synth", {"MonoLead", "MiniLead", "VS 89", "4%Pulse", "SynSine", "Sine"}},
                 {"Poly Synth", {"RawDeal", "Detune", "EtherBell", "FreshAir", "Ghostly", "Ephemerals", "AliaBass", "UnderWater", "Spectrum1", "Spectrum2", "Spectrum3"}},
                 {"SE/Other", {"Stadium", "Thing", "TriRoll", "Clicker", "Crickets1", "Crickets2", "MagicBell", "IronUp", "Tooter", "FluteFX", "Flutter"}},
                 {"Hit/Chord", {"OrchHit", "VibeHit", "Gong", "Timpani", "OrchPerc", "SynClaves", "SynTom1", "SynTom2", "Zap1", "Zap2", "Industry1", "Industry2", "RevThing"}},
                 {"Drum Kit", {"TotalKit", "RockKit", "DanceKit", "AnalogKit", "HipHopKit", "R&BKit", "BD&SDKit1", "BD&SDKit2", "TomKit", "CymbalKit", "PercKit1", "PercKit2", "SEKit"}},
             }},
    {"EX", {

               {"Keyboard", {"ElecGrand", "E.Piano1", "E.Piano2", "ToyPiano", "Organ", "VoxOrgan", "Musette"}},

               {"Guitar/Mallet", {"ChorusGtr", "JazzGuitar", "DistGuitar", "SitarTambr", "Shamisen", "Koto", "Glocken", "SteelDrum", "BottlePop"}},

               {"Bass", {"E.Bass", "SlapBass", "BoostSaw", "ElectroBs", "DarkBass", "FilterBass", "FatBass", "SawRezBass", "SquRezBass", "DiscoBass", "VPMBass1", "VPMBass2", "AttackBass", "AcidDistBs", "DetuneBass", "WobbleBass"}},

               {"Strings/Pad", {"Strings", "StrQuartet", "TapeString", "AahChoir", "OohChoir", "VocoderPad", "AnalogStr1", "AnalogStr2", "DarkPad", "NoisePad", "AnalogPad", "SquarePad", "5thPad", "OctavePad"}},

               {"Brass/Reed", {"Trumpet", "TinWhistle", "TapeFlute", "Shakuhachi", "AltoSax", "TenorSax", "Bagpipe", "BrassEns"}},

               {"Lead Synth", {"SoftLead", "UrbanLead", "HiResoLead", "MS20Lead", "OctaveLead", "DriveLead", "RaveLead", "DualSquare", "SynWire1", "SynWire2", "SyncLoop", "5thSine", "5thSaw", "5thSquare", "ShortArp"}},

               {"Poly Synth", {"DetuneStab", "UnisonStab", "5thStab", "PolyComp", "SquDetune", "VPMBrass", "SynthHorn", "DarkSynth", "NoisyComp", "RisingPad", "TremoloSin", "TrillPad", "LPFSweep", "HPFSweep", "WaveSweep", "Palawan"}},

               {"SE/Other", {"Applause", "HeartBeat", "GunShot", "CarSFX", "Stream", "Forest", "ShakerLoop", "Noise", "8bitNoise", "NoiseShot", "NoiseSplit", "SweepSplit", "SirenUp", "SirenDown", "Modulation", "Signal"}},

               {"Hit/Chord", {"M1PfChord", "EPChord", "min7Organ", "GtrChord1", "GtrChord2", "PowerChord", "StrChord", "Maj7Pad", "SynChord1", "SynChord2", "SynChord3", "SynChord4", "Glissando", "OrchHits", "DanceHits"}},

               {"Drum Kit", {"DDD1Kit", "DDM110Kit", "S3Kit", "LynKit", "StandrdKit", "HouseKit", "ElectroKit", "MinimalKit", "TronicaKit", "D&BKit", "R&BKit", "HipHopKit", "EthnicKit", "SEKit", "CartoonKit", "GtrElement"}},
           }}

};

constexpr int kNumberOfDrumNotes = 12;
constexpr int kDrumLowestNote = 60;
constexpr int kNoteOffset = 0x80;

InstrumentHelper::InstrumentHelper() : instrumentMap(defaultMap)
{
}

InstrumentName InstrumentHelper::GetInstrumentName(const uint8_t bankId, const uint8_t subBankId,
                                                   const uint8_t programId) const
{
    if (bankId > kNumberOfBanks || subBankId > kNumberOfSubBanks)
    {
        return InstrumentName("Error", "Error", "Error");
    }
    auto &bankName = kBankNames[bankId];
    auto &subBankName = kSubBankNames[subBankId];
    if (!instrumentMap.contains(bankName))
    {
        return InstrumentName("Unknown", "Unknown", "Unknown");
    }

    const auto &bank = instrumentMap.at(bankName);
    if (!bank.contains(subBankName))
    {
        return InstrumentName(bankName, "Unknown", "Unknown");
    }

    const auto &subBank = bank.at(subBankName);
    if (programId > subBank.size())
    {
        return InstrumentName(bankName, subBankName, "Unknown");
    }

    return InstrumentName(bankName, subBankName, subBank[programId]);
}

InstrumentIds InstrumentHelper::GetProgramChangeIds(const std::string &instrumentName) const
{
    for (const auto &[bankName, subBanks] : instrumentMap)
    {
        for (const auto &[subBankName, instruments] : subBanks)
        {
            for (int programId = 0; programId < instruments.size(); ++programId)
            {
                if (instruments[programId] == instrumentName)
                {
                    uint8_t bankId = 0;
                    uint8_t subBankId = 0;
                    for (const auto &name : std::views::keys(instrumentMap))
                    {
                        if (name == bankName)
                        {
                            break;
                        }
                        ++bankId;
                    }
                    for (const auto &name : std::views::keys(subBanks))
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

void InstrumentHelper::LoadConfigFile(const std::string &configPath)
{
    config_.clear();
    configLoaded_ = false;

    try
    {
        YAML::Node root = YAML::LoadFile(configPath);
        if (!root["Instruments"])
            return;

        YAML::Node instruments = root["Instruments"];
        for (auto itBank = instruments.begin(); itBank != instruments.end(); ++itBank)
        {
            const std::string &bankName = itBank->first.as<std::string>();
            YAML::Node bankNode = itBank->second;

            for (auto itSub = bankNode.begin(); itSub != bankNode.end(); ++itSub)
            {
                const std::string subBankName = itSub->first.as<std::string>();
                YAML::Node subNode = itSub->second;

                for (auto itProg = subNode.begin(); itProg != subNode.end(); ++itProg)
                {
                    const std::string programName = itProg->first.as<std::string>();
                    YAML::Node progNode = itProg->second;

                    InstrumentConfig cfg;

                    if (progNode["program"])
                        cfg.program = static_cast<uint8_t>(progNode["program"].as<int>());

                    if (progNode["channel"])
                        cfg.channel = static_cast<uint8_t>(progNode["channel"].as<int>());

                    if (progNode["transposition"])
                        cfg.transposition = progNode["transposition"].as<int>();

                    if (progNode["map"])
                    {
                        if (progNode["map"].IsSequence())
                        {
                            std::vector<uint8_t> seq;
                            for (const auto &v : progNode["map"])
                            {
                                seq.push_back(static_cast<uint8_t>(v.as<int>()));
                            }
                            cfg.mapList = std::move(seq);
                        }
                        else if (progNode["map"].IsMap())
                        {
                            std::map<int, uint8_t> mp;
                            for (auto it = progNode["map"].begin(); it != progNode["map"].end(); ++it)
                            {
                                int key = it->first.as<int>();
                                uint8_t val = static_cast<uint8_t>(it->second.as<int>());
                                mp.emplace(key, val);
                            }
                            cfg.mapDict = std::move(mp);
                        }
                    }

                    config_[bankName][subBankName][programName] = std::move(cfg);
                }
            }
        }

        configLoaded_ = true;
    }
    catch (const std::exception &)
    {
        configLoaded_ = false;
    }
}

InstrumentConfig InstrumentHelper::GetInstrumentConfig(const std::string &bankName,
                                                       const std::string &subBankName,
                                                       const std::string &programName) const
{
    InstrumentConfig empty;
    if (!configLoaded_)
        return empty;

    auto itBank = config_.find(bankName);
    if (itBank == config_.end())
        return empty;

    auto itSub = itBank->second.find(subBankName);
    if (itSub == itBank->second.end())
        return empty;

    auto itProg = itSub->second.find(programName);
    if (itProg == itSub->second.end())
        return empty;

    return itProg->second;
}

InstrumentConfig InstrumentHelper::GetInstrumentConfig(const uint8_t &bankId,
                                                       const uint8_t &subBankId,
                                                       const uint8_t &programId) const
{
    const auto &[bank, subBank, program] = GetInstrumentName(bankId, subBankId, programId);
    return GetInstrumentConfig(bank, subBank, program);
}

uint8_t InstrumentHelper::RemapNoteNumber(const InstrumentConfig &instrumentConfig, const uint8_t &noteNumber)
{
    uint8_t realNoteNumber = noteNumber - kNoteOffset;

    if (instrumentConfig.transposition.has_value())
    {
        realNoteNumber = std::max(0, std::min(127, realNoteNumber + instrumentConfig.transposition.value()));
    }

    if (!instrumentConfig.mapList.has_value() && !instrumentConfig.mapDict.has_value())
        return realNoteNumber;

    if (instrumentConfig.mapList.has_value())
    {
        const auto &list = *instrumentConfig.mapList;
        if (realNoteNumber < kDrumLowestNote || realNoteNumber >= kDrumLowestNote + kNumberOfDrumNotes)
        {
            return realNoteNumber;
        }

        const int index = (kNumberOfDrumNotes - 1) - (realNoteNumber - kDrumLowestNote);

        if (index < 0 || index >= list.size())
            return realNoteNumber;

        return list.at(index);
    }

    else if (instrumentConfig.mapDict.has_value())
    {
        const auto &dict = *instrumentConfig.mapDict;
        if (!dict.contains(realNoteNumber))
        {
            return realNoteNumber;
        }
        return dict.at(realNoteNumber);
    }

    return realNoteNumber;
}

uint8_t InstrumentHelper::MapRange(const long &value, const long &min, const long &max, const long &newMin,
                                   const long &newMax)
{
    return static_cast<uint8_t>((value - min) * (newMax - newMin) / (max - min) + newMin);
}
