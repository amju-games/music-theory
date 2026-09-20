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
// DON'T CHANGE THIS: Changing this would invalidate all config files.
static const char* XOR_KEY = R"(W vKC_e[ZSmvMXhBQ2_pp~C[wi~B+i=1[=KsB)oR%A8)'^UT'"1GvCtbx*m {ASZ|*FjvjuBGAw`1FE/u78BzJhx;X:d{dmgR`5yavuYmm.&V DornM'[Zy/w<|m[zcSj!{zh^ 'qg=e ~~1aJC!x<[8LZdDrKt4\{!-}c>`*C|!x:vCbiQ$(7|<:a&:.s`[4bAN=(~9@KAXNa72f,Ciq~cxMsf+SOe[/IoP5xb>y.=QSq}~yJF~d@et 5[q-&jC)o\\@zy%d<fx\Tl?9z-Hb|9}\kSK%n)<@Ma7D@NJ{FFpZfL)[,[a4;iDhVTe/z*ZANk*bzpm%E4 X0CAFt#^qxi3\vO\w+3"OgK,dVj|L8{!*nt.H-}r&qc/A|r>qdzpLAi%jK10a1S2u?.EP@?GH)j3(vfDn@T0?#xCM]SPCIaku01$1'[5K%RRmaOXSuy/?IWZdMIW!YW[AXP,IOTK`k:n}e %VsnDJGJ%\;x]Mqb6g{f",#mCd$:]nO882zTdV.ga4>4Mhx}5grr>;J^]\HN5J!\8*P\II/H?JUsL9gB3H(>fTG-iW>lenpqmj&{tX?llU`+eGaLDh-Gxr@SDOCi~&fCDJBSx\;xL&it^t45=#\?Gv$-!8'G?u"RbRc'uc]J;F1&!3NcD>%te,%|?aj]'b])3aey"D1u4p"664"h2tQCq\?y/yh:[*8xw~dN" /@ui>"M (aubnvf`[0!YW\?Fx%=T]Wnc|x5vv~&RQJ'dgY,Nj,^DbS#=0RKDNLlj-x# %,xy5{cBL-9f#74F!sP@B!!.=LOyW[>xbvz=Vx6XAsCJ,Awz1lw,~}#n7%\E\jh}AZe;f,5tP"d0Fo/qZsP,c#G#q>~v>wp'mc[Vw*N;XjK:RNo$n~"9wrAM1CI`![@pxw.5Ms`alxtu*'s.RM>Aw+N<s5=/9nbHOph&|YK[u7oyjmT^v)=&6x HGSXSP=*PS<X @ `HV)O2K^g]+%B~3F,I~m[c||&ybB7iD:zCQ+W6.l{$K<mh2n^tFi~)";  

class ObscureConfigFile : public ConfigFile 
{
public:
    explicit ObscureConfigFile(const std::string& fileXorKey = XOR_KEY)
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

