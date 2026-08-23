//
// Author: Mystievous <mystievous@starseekstudios.com>
// Creation Date: 5/17/2025
//

#ifndef INSTRUMENTHELPER_M
#define INSTRUMENTHELPER_M
#include <cstdint>
#include <optional>
#include <map>
#include <string>
#include <vector>

constexpr int kNumberOfBanks = 3;
constexpr int kNumberOfSubBanks = 10;

constexpr std::string kBankNames[] = {
    "M1",
    "01/W",
    "EX"
};

constexpr std::string kSubBankNames[] = {
    "Keyboard",
    "Guitar/Mallet",
    "Bass",
    "Strings/Pad",
    "Brass/Reed",
    "Lead Synth",
    "Poly Synth",
    "SE/Other",
    "Hit/Chord",
    "Drum Kit"
};

struct InstrumentName
{
    std::string bank;
    std::string subBank;
    std::string program;
};

struct InstrumentIds
{
    uint8_t bank;
    uint8_t subBank;
    uint8_t program;
};

struct InstrumentConfig
{
    std::optional<uint8_t> channel;
    std::optional<uint8_t> bank;
    std::optional<uint8_t> subBank;
    std::optional<uint8_t> program;
    std::optional<int> transposition;
    std::optional<std::vector<uint8_t>> mapList;
    std::optional<std::map<int, uint8_t>> mapDict;
};

typedef std::map<std::string, std::map<std::string, std::vector<std::string>>> InstrumentMap;

class InstrumentHelper
{
public:
    // Map of Banks, with SubBanks, and Instruments
    InstrumentMap instrumentMap;

    InstrumentHelper();

    [[nodiscard]] InstrumentName GetInstrumentName(uint8_t bankId, uint8_t subBankId, uint8_t programId) const;

    [[nodiscard]] InstrumentIds GetProgramChangeIds(const std::string& instrumentName) const;

    void LoadConfigFile(const std::string& configPath);

    [[nodiscard]] InstrumentConfig GetInstrumentConfig(const uint8_t& bankId,
                                                       const uint8_t& subBankId,
                                                       const uint8_t& programId) const;

    [[nodiscard]] InstrumentConfig GetInstrumentConfig(const std::string& bankName,
                                                       const std::string& subBankName,
                                                       const std::string& programName) const;

    static uint8_t RemapNoteNumber(const InstrumentConfig& instrumentConfig, const uint8_t& noteNumber);

    static uint8_t MapRange(const long& value, const long& min, const long& max, const long& newMin,
                            const long& newMax);

private:
    using ConfigTree = std::map<std::string, std::map<std::string, std::map<std::string, InstrumentConfig>>>;
    ConfigTree config_; // populated by LoadConfigFile
    bool configLoaded_ = false;
};

#endif // INSTRUMENTHELPER_M
