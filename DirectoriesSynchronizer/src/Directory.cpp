#include <filesystem>
#include <iostream>
#include "Directory.h"

Directory::Directory(const std::string& directoryPath)
	: m_Path(directoryPath)
{
	if (!std::filesystem::exists(directoryPath))
		LOG_ERROR("Directory: {0} does not exist!", directoryPath);

	LOG_DEBUG("Iterating through directory: {0}", directoryPath);
	for (const auto& element : std::filesystem::directory_iterator(directoryPath))
	{
		m_Elements.push_back({ directoryPath + "/" + element.path().filename().string(), (const unsigned int)element.file_size(), !element.path().filename().has_extension() });
	}
	LOG_DEBUG("Iteration complete");
}

