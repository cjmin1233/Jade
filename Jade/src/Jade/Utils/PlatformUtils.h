#pragma once
#include <string>

namespace Jade
{
    class FileDialogs
    {
    public:
        static std::optional<std::string> OpenFile(const char* filter = "All Files (*.*)");
        static std::optional<std::string> SaveFile(const char* filter = "All Files (*.*)");
    };
}
