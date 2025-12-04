#include "jdpch.h"

#include "Jade/Utils/Utils.h"

namespace Jade
{
    // Helper function to get file extension from a filepath
    const std::string Utils::GetFileExtension(const std::string& filepath)
    {
        size_t dotPosition = filepath.find_last_of('.');

        if (dotPosition == std::string::npos)
            return ""; // No extension found

        return filepath.substr(dotPosition + 1);
    }

    // Helper function to get file name without extension from a filepath
    const std::string Utils::GetFileName(const std::string& filepath)
    {
        size_t slashPosition = filepath.find_last_of("/\\");
        size_t dotPosition = filepath.find_last_of('.');

        size_t start = (slashPosition == std::string::npos) ? 0 : slashPosition + 1;
        size_t end = (dotPosition == std::string::npos || dotPosition < start) ? filepath.length() : dotPosition;

        return filepath.substr(start, end - start);
    }
}
