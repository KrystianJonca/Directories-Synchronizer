#include <filesystem>
#include <iostream>
#include "Directory.h"

Directory::Directory(const std::string& directoryPath)
	: m_Path(directoryPath)
{
	for (const auto& element : std::filesystem::directory_iterator(directoryPath))
	{
		m_Elements.push_back(DirElement{ directoryPath + "/" + element.path().filename().string(), (const unsigned int)element.file_size(), !element.path().filename().has_extension() });
	}
}

