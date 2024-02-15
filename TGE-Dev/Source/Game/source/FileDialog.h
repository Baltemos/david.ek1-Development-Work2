#pragma once
#include <Windows.h>
#include <string>
#include <filesystem>

struct FileDialogResult
{
	bool Succeeded = false;
	std::wstring Path;
};

FileDialogResult OpenFileDialog(const std::filesystem::path& aDefaultDirectory, const wchar_t* aDefaultExtension = L"", const wchar_t* aFilter = L"All Files (*.*)\0*.*");

FileDialogResult SaveFileDialog(const std::filesystem::path& aDefaultDirectory, const wchar_t* aDefaultExtension = L"", const wchar_t* aFilter = L"All Files (*.*)\0*.*");