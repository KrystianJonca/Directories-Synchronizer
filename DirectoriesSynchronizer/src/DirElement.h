#include <string>
#include <fstream>
#include <sstream>

#include "util/Logger.h"

struct DirElement
{
	std::string path;
	unsigned int size;
	bool isFolder;

	const std::string GetName() const
	{
		return path.substr(path.find_last_of("/") + 1);
	}
	const std::string GetContent() const
	{
		if (isFolder) return "Subfolder";

		std::string content;
		std::ifstream file;

		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			LOG_DEBUG("Reading: {0}", path);
			file.open(path);

			std::stringstream fileStream;

			fileStream << file.rdbuf();
			file.close();

			content = fileStream.str();
			LOG_DEBUG("Reading completed");
		}
		catch (std::ifstream::failure e)
		{
			LOG_ERROR("Could not load file: {0}", path);
		}
		return content;
	}
};
