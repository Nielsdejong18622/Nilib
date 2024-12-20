#ifndef _CONFIG_H
#define _CONFIG_H

#include "Core/Log.hpp"
#include <unordered_map>
#include "Core/Assert.h"
#include <optional>

class Config
{
public:
    using key_t = std::string;
    using value_t = std::variant<int, float, bool, unsigned int, size_t, std::string>;

    class Proxy
    {
    public:
        Proxy(Config* config, key_t section)
            : config(config), section(std::move(section)) {}

        Proxy operator[](key_t key) {
            this->key = std::move(key);
            return *this;
        }

        template<typename T>
        operator T() const {
            ASSERT(config->d_map.contains(section), std::format("Section {} does not exist in Configfile!", section))
            ASSERT(config->d_map[section].contains(key), std::format("Key {} does not exist in Configfile!", key))
            const auto& value = config->d_map.at(section).at(key);
            return std::get<T>(value);
        }

        template<typename T>
        Proxy& operator=(T&& value) {
            config->d_map[section][key] = std::forward<T>(value);
            return *this;
        }

        // Overload the << operator
        friend std::ostream& operator<<(std::ostream& os, const Proxy& proxy) {
            const auto& value = proxy.config->d_map.at(proxy.section).at(proxy.key);
            std::visit([&os](auto&& arg) { os << arg; }, value);
            return os;
        }

    private:
        Config* config;
        key_t section;
        key_t key;
    };

    Proxy operator[](const key_t& section) {
        if (d_map.find(section) == d_map.end())
            d_map[section] = {};
        return Proxy(this, section);
    }

    // Display all config fields. 
    void display() const {
        Log::info("Configuration ") << this << "-----\n";
        for (auto &[sec, v]  : d_map)
        {
            Log::info("----- ") << sec << " -----:\n";
            for (auto &[key, value] : v) 
            {    
                std::visit([&key, &sec, this](auto&& arg) { Log::info("\t") << sec << '.' << key << "=" << arg << '\n'; ; }, value);
            }
        }
        
    }

private:
    std::unordered_map<key_t, std::unordered_map<key_t, value_t>> d_map;
};



class ConfigFileParser 
{

public:


    static Config parse_from_ini(std::string const &filename) {
        Log::info("Parsing ini file ") << filename << " for config.\n";

        Config config;
        std::ifstream file(filename);
        if (!file.is_open()) {
            Log::warning("Could not open file: ") << filename << '\n';
            return config;
        }

        std::string line;
        std::string currentSection{"Global"};
        while (std::getline(file, line)) {
            line = trim(line);

            if (line.empty() || line[0] == ';' || line[0] == '#') {
                continue;
            }
            if (line.front() == '[' && line.back() == ']') {
                currentSection = line.substr(1, line.size() - 2);
            } else {
                size_t equalsPos = line.find('=');
                if (equalsPos != std::string::npos) {
                    std::string key = trim(line.substr(0, equalsPos));
                    std::string valueStr = trim(line.substr(equalsPos + 1));
                    Config::value_t value;

                    // Automatic type deduction
                    if (is_bool(valueStr)){
                        std::string lowerStr = valueStr;
                        std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
                        value = (lowerStr == "true") ? true : false;
                        //Log::debug("Parsing bool ") << currentSection << '.' << key << '=' << valueStr << '\n';
                    } else if (is_uint(valueStr)) {
                        value = std::stoull(valueStr);
                        //Log::debug("Parsing size_t ") << currentSection << '.' << key << '=' << valueStr << '\n';
                    } else if (is_int(valueStr)) {
                        value = std::stoi(valueStr);
                        //Log::debug("Parsing int ") << currentSection << '.' << key << '=' << valueStr << '\n';
                    } else if (is_float(valueStr)) {
                        value = std::stof(valueStr);
                        //Log::debug("Parsing float ") << currentSection << '.' << key << '=' << valueStr << '\n';
                    } else{
                        //Log::debug("Parsing str ") << currentSection << '.' << key << '=' << valueStr << '\n';
                        value = valueStr;
                    }
                    config[currentSection][key] = value;
                }
            }
        }
        return config;
    }

private:

    static std::string trim(std::string const &str) {
        size_t first = str.find_first_not_of(" \t");
        size_t last = str.find_last_not_of(" \t");
        return (first == std::string::npos || last == std::string::npos) ? "" : str.substr(first, last - first + 1);
    }
    static bool is_uint(std::string const &str) {
        char* end = nullptr;
        long val = std::strtoull(str.c_str(), &end, 10);
        return end != str.c_str() && *end == '\0' && val <= INT_MAX && val >= INT_MIN;
    }
    static bool is_int(std::string const &str) {
        char* end = nullptr;
        long val = std::strtol(str.c_str(), &end, 10);
        return end != str.c_str() && *end == '\0' && val <= INT_MAX && val >= INT_MIN;
    }
    static bool is_bool(std::string const &str) {
        std::string lowerStr = str;
        std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
        return (lowerStr == "true" || lowerStr == "false");
    }

    static bool is_float(std::string const &str) {
        char* end = nullptr;
        std::strtof(str.c_str(), &end);
        return end != str.c_str() && *end == '\0';
    }
};

#endif