#pragma once

#include "ConfigFile.h"
#include <string>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <cstdlib>
#include <ctime>

namespace Amju 
{
class ObscureConfigFile : public ConfigFile 
{
public:
    explicit ObscureConfigFile(const std::string& fileXorKey = "AmjuDefaultSecretKey123")
        : m_fileXorKey(fileXorKey) 
    {
        // Generate a random 32-bit session mask for memory obfuscation
        std::srand(static_cast<unsigned int>(std::time(NULL)));
        m_memSessionMask = (static_cast<uint32_t>(std::rand()) << 16) ^ static_cast<uint32_t>(std::rand());
    }

    static std::string HashKey(const std::string& key) 
    {
        uint32_t hash = 2166136261u;
        for (char c : key) 
        {
            hash ^= static_cast<uint8_t>(c);
            hash *= 16777619u;
        }
        return std::to_string(hash);
    }

    static uint32_t CalculateCRC32(const std::string& data) 
    {
        uint32_t crc = 0xFFFFFFFFu;
        for (char c : data) 
        {
            uint8_t byte = static_cast<uint8_t>(c);
            crc ^= byte;
            for (int i = 0; i < 8; ++i) 
            {
                if (crc & 1)
                    crc = (crc >> 1) ^ 0xEDB88320u;
                else
                    crc >>= 1;
            }
        }
        return ~crc;
    }

    // Encrypts/decrypts in-memory data using the runtime session mask
    std::string ObfuscateMemory(const std::string& input) const 
    {
        std::string result = input;
        const uint8_t* maskBytes = reinterpret_cast<const uint8_t*>(&m_memSessionMask);
        for (size_t i = 0; i < result.size(); ++i) 
        {
            result[i] ^= maskBytes[i % sizeof(m_memSessionMask)];
        }
        return result;
    }

    // Overridden Set: hashes key and obfuscates value in memory
    void Set(const std::string& key, const std::string& value) 
    {
        std::string hashedKey = HashKey(key);
        std::string obfuscatedValue = ObfuscateMemory(value);
        ConfigFile::Set(hashedKey, obfuscatedValue);
    }

    // Overridden GetValue: fetches obfuscated value and decrypts it
    std::string GetValue(const std::string& key, const std::string& defaultVal = "") const 
    {
        std::string hashedKey = HashKey(key);
        if (!ConfigFile::Exists(hashedKey)) 
        {
            return defaultVal;
        }
        // Default value not used in next line: we already checked key
        std::string obfuscatedValue = ConfigFile::GetValue(hashedKey, "");
        return ObfuscateMemory(obfuscatedValue);
    }

    bool Exists(const std::string& key) const 
    {
        return ConfigFile::Exists(HashKey(key));
    }

    void Erase(const std::string& key) 
    {
        ConfigFile::Erase(HashKey(key));
    }

    std::string XorFileBuffer(const std::string& data) const 
    {
        if (m_fileXorKey.empty()) return data;
        std::string result = data;
        for (size_t i = 0; i < data.size(); ++i) 
        {
            result[i] ^= m_fileXorKey[i % m_fileXorKey.size()];
        }
        return result;
    }

    bool SaveObscured(const std::string& filename) 
    {
        // De-obfuscate memory values prior to serializing to disk format
        std::ostringstream oss;
        for (ConfigMap::const_iterator it = m_values.begin(); it != m_values.end(); ++it) 
        {
            std::string plainValue = ObfuscateMemory(it->second);
            oss << it->first << "=" << plainValue << "\n";
        }

        std::string plainText = oss.str();
        std::string encrypted = XorFileBuffer(plainText);
        uint32_t checksum = CalculateCRC32(encrypted);

        std::ofstream outFile(filename.c_str(), std::ios::binary);
        if (!outFile) return false;

        outFile.write(reinterpret_cast<const char*>(&checksum), sizeof(checksum));
        outFile.write(encrypted.data(), encrypted.size());

        m_isDirty = false;
        return outFile.good();
    }

    bool LoadObscured(const std::string& filename) 
    {
        std::ifstream inFile(filename.c_str(), std::ios::binary | std::ios::ate);
        if (!inFile) return false;

        std::streamsize totalSize = inFile.tellg();
        if (totalSize < static_cast<std::streamsize>(sizeof(uint32_t))) return false;

        inFile.seekg(0, std::ios::beg);

        uint32_t storedChecksum = 0;
        inFile.read(reinterpret_cast<char*>(&storedChecksum), sizeof(storedChecksum));

        std::streamsize payloadSize = totalSize - sizeof(uint32_t);
        std::string buffer(payloadSize, '\0');
        if (!inFile.read(&buffer[0], payloadSize)) return false;

        if (storedChecksum != CalculateCRC32(buffer)) return false;

        std::string decrypted = XorFileBuffer(buffer);
        std::istringstream iss(decrypted);

        Clear();
        std::string line;
        while (std::getline(iss, line)) 
        {
            size_t delimPos = line.find('=');
            if (delimPos != std::string::npos) 
            {
                std::string k = line.substr(0, delimPos);
                std::string plainVal = line.substr(delimPos + 1);
                // Re-obfuscate into memory storage
                m_values[k] = ObfuscateMemory(plainVal);
            }
        }
        return true;
    }

    // Direct access to underlying map for unit test inspection
    const ConfigMap& GetInternalMap() const 
    {
        return m_values;
    }

private:
    // Use LoadObscured/SaveObscured
    using ConfigFile::Save;
    using ConfigFile::Load;

private:
    std::string m_fileXorKey;
    uint32_t m_memSessionMask;
};

// Use this to get the global writable game config.
ObscureConfigFile& GetObscureConfigFile();
} // namespace Amju

