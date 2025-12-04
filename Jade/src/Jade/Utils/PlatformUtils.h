#pragma once
#include <string>

namespace Jade
{
    class FileDialogs
    {
    public:
        static std::string OpenFile(const char* filter = "All Files (*.*)");
        static std::string SaveFile(const char* filter = "All Files (*.*)");
    };
}
