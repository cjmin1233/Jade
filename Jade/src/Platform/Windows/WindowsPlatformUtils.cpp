#include "jdpch.h"

#include "Jade/Utils/PlatformUtils.h"
#include "Jade/Core/Application.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#ifndef GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#include <GLFW/glfw3native.h>

namespace Jade
{
    std::optional<std::string> FileDialogs::OpenFile(const char* filter)
    {
        OPENFILENAMEA ofn;              // common dialog box structure
        CHAR szFile[260] = { 0 };       // buffer for file name
        CHAR currentDir[256] = { 0 };   // buffer for current directory

        ZeroMemory(&ofn, sizeof(ofn));

        // Fill in the OPENFILENAME structure
        ofn.lStructSize = sizeof(ofn);  // size of structure
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());  // owner window
        ofn.lpstrFilter = filter;   // filter string
        ofn.nFilterIndex = 1;   // index of filter
        ofn.lpstrFile = szFile; // buffer for file name
        ofn.nMaxFile = sizeof(szFile);  // maximum file name size
        ofn.lpstrTitle = "Open File";  // title of dialog
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;  // dialog options

        // Get the current directory to set as the initial directory
        if (GetCurrentDirectoryA(sizeof(currentDir), currentDir))
        {
            ofn.lpstrInitialDir = currentDir;
        }

        // Display the Open dialog box
        if (GetOpenFileNameA(&ofn))
        {
            // If the user selected a file, return its path
            return std::string(ofn.lpstrFile);
        }

        // If the user canceled the dialog or an error occurred, return std::nullopt
        return std::nullopt;
    }

    std::optional<std::string> FileDialogs::SaveFile(const char* filter)
    {
        OPENFILENAMEA ofn;  // common dialog box structure
        CHAR szFile[260] = { 0 };   // buffer for file name
        CHAR currentDir[256] = { 0 };   // buffer for current directory

        ZeroMemory(&ofn, sizeof(ofn));

        // Fill in the OPENFILENAME structure
        ofn.lStructSize = sizeof(ofn);  // size of structure
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());  // owner window
        ofn.lpstrFilter = filter;   // filter string
        ofn.nFilterIndex = 1;   // index of filter
        ofn.lpstrFile = szFile; // buffer for file name
        ofn.nMaxFile = sizeof(szFile);  // maximum file name size
        ofn.lpstrTitle = "Save File";  // title of dialog
        // - OFN_NOREADONLYRETURN: The dialog will not return read-only files.
        // - OFN_HIDEREADONLY: The read-only checkbox is hidden.
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR
            | OFN_NOREADONLYRETURN | OFN_HIDEREADONLY;  // dialog options

        // Set default extension based on the first filter's extension
        ofn.lpstrDefExt = std::strchr(filter, '\0') + 1;

        // Get the current directory to set as the initial directory
        if (GetCurrentDirectoryA(sizeof(currentDir), currentDir))
        {
            ofn.lpstrInitialDir = currentDir;
        }

        // Display the Save dialog box
        if (GetSaveFileNameA(&ofn))
        {
            // If the user selected a file, return its path
            return std::string(ofn.lpstrFile);
        }

        // If the user canceled the dialog or an error occurred, return std::nullopt
        return std::nullopt;
    }
}
