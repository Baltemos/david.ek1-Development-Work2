#include "FileDialog.h"
#include <tge/util/StringCast.h>
#include <filesystem>
#include <tge/util/StringCast.h>

FileDialogResult OpenFileDialog(const std::filesystem::path& aDefaultDirectory, const wchar_t* aDefaultExtension, const wchar_t* aFilter)
{
    FileDialogResult result;

    wchar_t fileName[MAX_PATH];
    ZeroMemory(fileName, sizeof(fileName));

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = GetActiveWindow();

    std::filesystem::path defaultDirectory = std::filesystem::current_path() / aDefaultDirectory;
    ofn.lpstrInitialDir = defaultDirectory.c_str();
    ofn.lpstrFilter = aFilter;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = sizeof(fileName);
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
    ofn.lpstrDefExt = aDefaultExtension;

    result.Succeeded = GetOpenFileName(&ofn);
    if (result.Succeeded)
    {
        result.Path = std::wstring(fileName, sizeof(fileName));
    }

    return result;
}

FileDialogResult SaveFileDialog(const std::filesystem::path& aDefaultDirectory, const wchar_t* aDefaultExtension, const wchar_t* aFilter)
{
    FileDialogResult result;

    wchar_t fileName[MAX_PATH];
    ZeroMemory(fileName, sizeof(fileName));

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = GetActiveWindow();

    std::filesystem::path defaultDirectory = std::filesystem::current_path() / aDefaultDirectory;
    ofn.lpstrInitialDir = defaultDirectory.c_str();
    ofn.lpstrFilter = aFilter;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = sizeof(fileName);
    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
    ofn.lpstrDefExt = aDefaultExtension;

    result.Succeeded = GetSaveFileName(&ofn);
    if (result.Succeeded)
    {
        result.Path = std::wstring(fileName, sizeof(fileName));
    }

    return result;
}
