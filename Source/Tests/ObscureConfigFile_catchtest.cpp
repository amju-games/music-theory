#include <cstdio>
#include <fstream>
#include "catch.hpp"
#include "ObscureConfigFile.h"

TEST_CASE("ObscureConfigFile key hashing and getter/setter access", "[ObscureConfigFile]") 
{
    Amju::ObscureConfigFile config("MyGameKey");

    SECTION("Set and retrieve typed data") 
    {
        config.Set("HighScore", "1500");
        config.SetInt("PlayerLives", 3);
        config.SetFloat("Volume", 0.75f);

        REQUIRE(config.GetValue("HighScore") == "1500");
        REQUIRE(config.GetInt("PlayerLives") == 3);
        REQUIRE(config.GetFloat("Volume") == Approx(0.75f));
    }

    SECTION("Key presence check") 
    {
        config.Set("UnlockedLevel2", "true");
        REQUIRE(config.Exists("UnlockedLevel2"));
        REQUIRE_FALSE(config.Exists("UnlockedLevel3"));
    }

    SECTION("Key removal via Erase") 
    {
        config.Set("TempData", "999");
        REQUIRE(config.Exists("TempData"));
        config.Erase("TempData");
        REQUIRE_FALSE(config.Exists("TempData"));
    }
}

TEST_CASE("ObscureConfigFile XOR transformation property", "[ObscureConfigFile]") 
{
    Amju::ObscureConfigFile config("SecretKey123");

    SECTION("XOR operation is fully reversible") 
    {
        std::string plainText = "HighScore=1000\nGold=50\n";
        std::string obscured = config.XorFileBuffer(plainText);
        std::string restored = config.XorFileBuffer(obscured);

        REQUIRE(obscured != plainText);
        REQUIRE(restored == plainText);
    }
}

TEST_CASE("ObscureConfigFile file save and load cycle", "[ObscureConfigFile]") 
{
    const std::string testFile = "test_obscured_config.bin";
    
    SECTION("SaveObscured writes payload and LoadObscured restores original data") 
    {
        Amju::ObscureConfigFile saveConfig("GameSecretPass");
        saveConfig.Set("Coins", "250");
        saveConfig.SetInt("Level", 5);
        
        REQUIRE(saveConfig.SaveObscured(testFile));

        Amju::ObscureConfigFile loadConfig("GameSecretPass");
        REQUIRE(loadConfig.LoadObscured(testFile));

        REQUIRE(loadConfig.GetValue("Coins") == "250");
        REQUIRE(loadConfig.GetInt("Level") == 5);

        std::remove(testFile.c_str());
    }

    SECTION("Loading with incorrect XOR key fails decoding") 
    {
        Amju::ObscureConfigFile saveConfig("CorrectKey");
        saveConfig.Set("Coins", "250");
        saveConfig.SaveObscured(testFile);

        Amju::ObscureConfigFile loadConfig("WrongKey");
        loadConfig.LoadObscured(testFile);

        REQUIRE_FALSE(loadConfig.GetValue("Coins") == "250");

        std::remove(testFile.c_str());
    }
}

TEST_CASE("CRC32 calculation consistency", "[ObscureConfigFile]") 
{
    SECTION("Same input yields deterministic CRC32") 
    {
        std::string data = "Score=100\nGold=500\n";
        uint32_t crc1 = Amju::ObscureConfigFile::CalculateCRC32(data);
        uint32_t crc2 = Amju::ObscureConfigFile::CalculateCRC32(data);

        REQUIRE(crc1 == crc2);
    }

    SECTION("Different input yields different CRC32") 
    {
        uint32_t crc1 = Amju::ObscureConfigFile::CalculateCRC32("Score=100");
        uint32_t crc2 = Amju::ObscureConfigFile::CalculateCRC32("Score=900");

        REQUIRE(crc1 != crc2);
    }
}

TEST_CASE("ObscureConfigFile anti-tampering verification", "[ObscureConfigFile]") 
{
    const std::string filename = "test_tamper_config.bin";

    SECTION("Valid saved file passes checksum validation") 
    {
        Amju::ObscureConfigFile saveConfig("GameKey123");
        saveConfig.Set("XP", "4500");
        REQUIRE(saveConfig.SaveObscured(filename));

        Amju::ObscureConfigFile loadConfig("GameKey123");
        REQUIRE(loadConfig.LoadObscured(filename));
        REQUIRE(loadConfig.GetValue("XP") == "4500");

        std::remove(filename.c_str());
    }

    SECTION("Fails to load when a single byte in the file payload is modified") 
    {
        Amju::ObscureConfigFile saveConfig("GameKey123");
        saveConfig.Set("Coins", "100");
        REQUIRE(saveConfig.SaveObscured(filename));

        // Tamper with one byte inside the file after header
        std::fstream file(filename.c_str(), std::ios::in | std::ios::out | std::ios::binary);
        REQUIRE(file.is_open());
        
        file.seekp(sizeof(uint32_t) + 2, std::ios::beg); // Skip 4-byte CRC header and modify byte 2
        char tamperedByte = 0xFF;
        file.write(&tamperedByte, 1);
        file.close();

        // Attempt loading modified file
        Amju::ObscureConfigFile loadConfig("GameKey123");
        REQUIRE_FALSE(loadConfig.LoadObscured(filename));

        std::remove(filename.c_str());
    }

    SECTION("Fails to load when file size is truncated below header size") 
    {
        std::ofstream file(filename.c_str(), std::ios::binary);
        char tinyBuffer[2] = {0x01, 0x02};
        file.write(tinyBuffer, 2);
        file.close();

        Amju::ObscureConfigFile loadConfig("GameKey123");
        REQUIRE_FALSE(loadConfig.LoadObscured(filename));

        std::remove(filename.c_str());
    }
}

TEST_CASE("In-memory value obfuscation against memory scanning", "[ObscureConfigFile]") 
{
    Amju::ObscureConfigFile config("FileSecretKey");

    SECTION("Public API returns correct plaintext value") 
    {
        config.Set("Gold", "500");
        config.SetInt("Health", 100);

        REQUIRE(config.GetValue("Gold") == "500");
        REQUIRE(config.GetInt("Health") == 100);
    }

    SECTION("Internal map does NOT store readable plaintext strings") 
    {
        config.Set("Gold", "50000");

        std::string hashedKey = Amju::ObscureConfigFile::HashKey("Gold");
        const auto& map = config.GetInternalMap();

        REQUIRE(map.find(hashedKey) != map.end());

        std::string rawStoredValue = map.at(hashedKey);
        
        // Ensure memory contents differ from the original string
        REQUIRE(rawStoredValue != "50000");
    }
}

TEST_CASE("ObscureConfigFile default value handling for missing keys", "[ObscureConfigFile]")
{
    Amju::ObscureConfigFile config("TestKey123");

    SECTION("GetValue returns explicit default when key does not exist")
    {
        REQUIRE(config.GetValue("NonExistentKey", "DefaultValue") == "DefaultValue");
    }

    SECTION("GetValue returns empty string when no default is provided")
    {
        REQUIRE(config.GetValue("MissingKey") == "");
    }

    SECTION("GetInt returns fallback default when key is missing")
    {
        // Default parameter is 0
        REQUIRE(config.GetInt("MissingInt") == 0);
        // Explicit default parameter
        REQUIRE(config.GetInt("MissingIntWithDefault", 42) == 42);
    }

    SECTION("GetFloat returns fallback default when key is missing")
    {
        // Default parameter is 0.0f
        REQUIRE(config.GetFloat("MissingFloat") == Approx(0.0f));
        // Explicit default parameter
        REQUIRE(config.GetFloat("MissingFloatWithDefault", 3.14f) == Approx(3.14f));
    }

    SECTION("Returns user data when key exists rather than default")
    {
        config.Set("ExistingKey", "ActualValue");
        config.SetInt("ExistingInt", 100);

        REQUIRE(config.GetValue("ExistingKey", "DefaultValue") == "ActualValue");
        REQUIRE(config.GetInt("ExistingInt", 42) == 100);
    }
}

TEST_CASE("ObscureConfigFile edge cases: empty strings, delimiter symbols, and binary data", "[ObscureConfigFile]")
{
    const std::string testFile = "test_edge_cases.bin";

    SECTION("Empty key and empty value handling")
    {
        Amju::ObscureConfigFile config("TestKey123");

        config.Set("", "emptyKeyVal");
        config.Set("emptyValKey", "");
        config.Set("", "");

        // Verify in-memory access for empty strings
        REQUIRE(config.GetValue("") == "");
        REQUIRE(config.GetValue("emptyValKey") == "");
        REQUIRE(config.Exists("emptyValKey"));
        REQUIRE(config.Exists(""));

        // Verify file serialization cycle with empty strings
        REQUIRE(config.SaveObscured(testFile));

        Amju::ObscureConfigFile loadedConfig("TestKey123");
        REQUIRE(loadedConfig.LoadObscured(testFile));

        REQUIRE(loadedConfig.GetValue("emptyValKey") == "");
        REQUIRE(loadedConfig.GetValue("") == "");

        std::remove(testFile.c_str());
    }

    SECTION("Values containing multiple '=' delimiter characters")
    {
        Amju::ObscureConfigFile config("TestKey123");
        std::string valWithEquals = "setting=123=abc==";

        config.Set("Formula", valWithEquals);
        REQUIRE(config.GetValue("Formula") == valWithEquals);

        REQUIRE(config.SaveObscured(testFile));

        Amju::ObscureConfigFile loadedConfig("TestKey123");
        REQUIRE(loadedConfig.LoadObscured(testFile));

        // Verifies line.substr(delimPos + 1) captures all sub-string '=' signs correctly
        REQUIRE(loadedConfig.GetValue("Formula") == valWithEquals);

        std::remove(testFile.c_str());
    }

    SECTION("Binary data, non-ASCII, and control characters")
    {
        Amju::ObscureConfigFile config("TestKey123");

        // Construct a raw binary payload containing null bytes, high ASCII, and symbols
        std::string binaryValue;
        binaryValue.push_back('\x00');
        binaryValue.push_back('\x1F');
        binaryValue.push_back('\x7F');
        binaryValue.push_back('\xFF');
        binaryValue.append("!@#$%^&*()_+-=[]{}|;':\",./<>?");

        config.Set("BinaryData", binaryValue);
        REQUIRE(config.GetValue("BinaryData") == binaryValue);

        REQUIRE(config.SaveObscured(testFile));

        Amju::ObscureConfigFile loadedConfig("TestKey123");
        REQUIRE(loadedConfig.LoadObscured(testFile));

        REQUIRE(loadedConfig.GetValue("BinaryData") == binaryValue);

        std::remove(testFile.c_str());
    }

    SECTION("Whitespace-only keys and values (tabs, spaces)")
    {
        Amju::ObscureConfigFile config("TestKey123");

        config.Set("  spaced_key  ", "  spaced value  ");
        config.Set("tab_key", "\t\tvalue_with_tabs\t");

        REQUIRE(config.GetValue("  spaced_key  ") == "  spaced value  ");
        REQUIRE(config.GetValue("tab_key") == "\t\tvalue_with_tabs\t");

        REQUIRE(config.SaveObscured(testFile));

        Amju::ObscureConfigFile loadedConfig("TestKey123");
        REQUIRE(loadedConfig.LoadObscured(testFile));

        REQUIRE(loadedConfig.GetValue("  spaced_key  ") == "  spaced value  ");
        REQUIRE(loadedConfig.GetValue("tab_key") == "\t\tvalue_with_tabs\t");

        std::remove(testFile.c_str());
    }
}

TEST_CASE("ObscureConfigFile m_isDirty flag state assertions", "[ObscureConfigFile]")
{
    const std::string testFile = "test_dirty_flag_explicit.bin";
    std::remove(testFile.c_str());

    SECTION("Newly constructed instance starts clean")
    {
        Amju::ObscureConfigFile config("TestSecretKey");

        REQUIRE_FALSE(config.IsDirty());

        // SaveObscured returns true without writing to disk
        REQUIRE(config.SaveObscured(testFile));

        std::ifstream fileCheck(testFile.c_str());
        REQUIRE_FALSE(fileCheck.is_open());
    }

    SECTION("Set transitions IsDirty from false to true")
    {
        Amju::ObscureConfigFile config("TestSecretKey");
        REQUIRE_FALSE(config.IsDirty());

        config.Set("PlayerName", "Hero");

        REQUIRE(config.IsDirty());
    }

    SECTION("SaveObscured resets IsDirty back to false")
    {
        Amju::ObscureConfigFile config("TestSecretKey");
        config.Set("PlayerName", "Hero");
        REQUIRE(config.IsDirty());

        REQUIRE(config.SaveObscured(testFile));

        REQUIRE_FALSE(config.IsDirty());

        std::remove(testFile.c_str());
    }

    SECTION("Modifying existing key sets IsDirty back to true")
    {
        Amju::ObscureConfigFile config("TestSecretKey");
        config.Set("Difficulty", "Normal");
        config.SaveObscured(testFile);

        REQUIRE_FALSE(config.IsDirty());

        // Updating existing value marks dirty again
        config.Set("Difficulty", "Hard");

        REQUIRE(config.IsDirty());

        std::remove(testFile.c_str());
    }

    SECTION("LoadObscured leaves instance clean upon loading")
    {
        // Create initial file on disk
        {
            Amju::ObscureConfigFile saveConfig("TestSecretKey");
            saveConfig.Set("Volume", "100");
            saveConfig.SaveObscured(testFile);
        }

        Amju::ObscureConfigFile loadConfig("TestSecretKey");
        REQUIRE_FALSE(loadConfig.IsDirty());

        REQUIRE(loadConfig.LoadObscured(testFile));

        // After successful load, flag must remain false
        REQUIRE_FALSE(loadConfig.IsDirty());

        std::remove(testFile.c_str());
    }
}

