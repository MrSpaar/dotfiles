#pragma once
#include <string>
#include <sstream>


inline std::string trim(const std::string& str) {
    const char* whitespace = " \t\n\r\f\v";
    size_t start = str.find_first_not_of(whitespace);
    size_t end = str.find_last_not_of(whitespace);

    if (start == std::string::npos || end == std::string::npos) {
        return "";
    }

    return str.substr(start, end - start + 1);
}


inline std::pair<std::string, std::string> parseConfigLine(std::string &line) {
    line = trim(line);

    if (line.empty() || line[0] == '[' || line.find('=') == std::string::npos) {
        return {};
    }

    std::stringstream ss(line);
    std::string key, value;

    if (std::getline(ss, key, '=') && std::getline(ss, value)) {
        return make_pair(key, trim(value));
    }

    return {};
}
