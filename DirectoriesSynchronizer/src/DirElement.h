#include <string>
#include <fstream>
#include <sstream>

struct DirElement
{
	const std::string path;
	const unsigned int size;
	const bool isFolder;

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
			file.open(path);

			std::stringstream fileStream;

			fileStream << file.rdbuf();
			file.close();

			content = fileStream.str();
		}
		catch (std::ifstream::failure e)
		{
			// TODO: ERROR LOGGER
		}
		return content;
	}
};
