#pragma once

#include <vector>
#include <string>
#include <map>

namespace CLI
{
    std::vector<std::string> ConstructArgumentList(int argc, char** argv);

    std::vector<std::string> RemoveOptions(std::vector<std::string>& args);

    // Tries to parse the options in the form of -<key>=<value>
    std::map<std::string, std::string> ParseOptions(const std::vector<std::string>& args);
} // namespace CLI
